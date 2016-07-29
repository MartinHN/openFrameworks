#pragma once

#include <vulkan/vulkan.h>
#include "vk/Shader.h"
#include "vk/Pipeline.h"
#include "ofMatrix4x4.h"
#include "ofMesh.h"

/// Context manages all transient state
/// + transformation matrices
/// + material 
/// + geometry bindings
/// transient state is tracked and accumulated in CPU memory
/// before frame submission, state is flushed to GPU memory

/*

Context exists to provide legacy support for immediate mode 
style rendering.

You draw inside a context and can expect it to work
in a similar way to OpenGL immediate mode. But without the OpenGL 
"under the hood" driver optimisations.

It may be possible to use context to pre-record memory
and command buffers - and to use this to playback "canned" 
command buffers.

For this to work, you would use a static context - a context 
with one frame of backing memory - which is transferred from 
host memory to GPU memory before being used to draw.

*/


class ofVkRenderer;

namespace of {
namespace vk {

class Allocator; // ffdecl.

/// \brief  Context stores any transient data
/// \detail Context tracks state between begin() and end(), mimicking 
///         legacy "immediate mode" renderer behaviour
///
/// The context holds a number of frames, dependent on the 
/// number of swapchain images. For each swapchain image,
/// there is a state memory frame within the context. 
///
/// The context has one allocator, which holds one buffer which is backed
/// by one large chunk device memory. Device memory is segmented into 
/// equal sized parts, one part for each memory frame.
///
/// You tell the context which frame to operate on by passing the swapchain 
/// image index when calling Context::begin()

class Context
{
	shared_ptr<of::vk::Allocator> mAlloc;

	// dynamic offsets for descriptor bindings, flattened by set
	// so that we can feed this directly to vkCmdBindDescriptorSets
	std::vector<std::vector<uint32_t>> mDynamicUniformBuffferOffsets;	  	   // VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC

	// -----------	Frame state CPU memory backing

	// TODO: rename to UboBufferData
	struct UniformBufferData
	{
		int32_t  stackId = -1;
		uint32_t memoryOffset = 0;	// gpu memory offset once data has been stored
		std::vector<uint8_t> data;  // this is the data - size of this vector depends on struct_size received from spirV-cross, this is the size for the whole binding / for the whole ubo struct.
	};

	// TODO: rename to UboBufferStack
	struct UniformBufferState
	{
		uint32_t bindingId   =  0;    // binding id within set
		uint32_t struct_size =  0;    // size in bytes of UniformBufferData.data vec
		int32_t  lastSavedStackId = -1;    // rolling count of elements saved to stack
		UniformBufferData state;
		
		std::string name; // name for this UniformBuffer Block
		std::list<UniformBufferData> stateStack;
		
		void push(){
			stateStack.push_back( state );
			state.stackId = -1;
		}
		
		void pop(){
			if ( stateStack.empty() ){
				ofLog() << "cannot pop empty uniform buffer state";
				return;
			}
			state = stateStack.back();
			stateStack.pop_back();			
		}

	};

	// TODO: rename to UboMemberBacking
	struct UniformMember   // sub-element of a binding witin a set
	{
		// the important thing here is that a binding can have multiple uniforms
		// (or UBO members as they are called in SPIR-V)

		// but one uniform can only belong to one binding.

		UniformBufferState* buffer; // this points to the buffer that will be affected by this binding - there is one buffer for each binding  - 
									// this is the index into the bufferOffsets vector for the shader layout this binding belongs to.

		uint32_t offset;
		uint32_t range;
	};

	struct DescriptorSetState
	{
		// all bindings associated with this descriptor set
		std::list<UniformBufferState> bindings;
		// GPU memory offset for all currently bound descriptors 
		std::vector<uint32_t>  bindingOffsets;
	};

	struct Frame
	{
		// map from descriptor set id to descriptor set state
		std::map < uint64_t, DescriptorSetState > mUniformBufferState;
		// map from uniform name to set and binding for uniform
		std::map<std::string, UniformMember> mUniformMembers;
	};

	// We hold stacks of CPU memory as a backing and readsource for lazy GPU uploads on draw
	// for all currently bound descriptorsets
	Frame mCurrentFrameState;

	// -----------


	int mSwapIdx = 0;


	// --------- pipeline info

	VkPipelineCache       mPipelineCache;

	// object which tracks current pipeline state
	// and creates a pipeline.
	
	GraphicsPipelineState mCurrentGraphicsPipelineState;


	//// currently bound shader
	std::shared_ptr<of::vk::Shader> mCurrentShader; 

	struct DescriptorSetLayoutInfo
	{
		of::vk::Shader::SetLayout setLayout;
		VkDescriptorSetLayout     vkDescriptorSetLayout;
	};

	std::vector<VkDescriptorSet> getBoundDescriptorSets();

	VkPipeline mCurrentPipeline = nullptr;

	// map of descriptorsetlayoutKey to descriptorsetlayout
	// this map is the central registry of DescriptorSetLayouts for all Shaders
	// used with(in) this Context.
	std::map<uint64_t, std::shared_ptr<DescriptorSetLayoutInfo>> mDescriptorSetLayouts;

	// sets up backing memory to track state, based on shaders
	void setupFrameState();

	// pool where all descriptors of this context are allocated from
	VkDescriptorPool mDescriptorPool = nullptr;

	void setupDescriptorPool( );
	
	std::vector<std::shared_ptr<of::vk::Shader>> mShaders;

public:

	struct Settings
	{
		VkDevice      device = nullptr;
		size_t        numSwapchainImages = 0 ;
		VkRenderPass  renderPass = nullptr;
		// context is initialised with a vector of shaders
		// all these shaders contribute to the shared pipeline layout 
		// for this context. The shaders need to be compatible in their
		// sets/bindings so that there can be a shared pipeline layout 
		// for the whole context.
	} const mSettings;

	// must be constructed with this method, default constructor
	// copy, and move constructor
	// have been implicitly deleted by defining mSettings const
	Context( const of::vk::Context::Settings& settings_ );

	~Context(){
		reset();
	}

	void addShader( std::shared_ptr<of::vk::Shader> shader_ );

	// allocates memory on the GPU for each swapchain image (call rarely)
	void setup( ofVkRenderer* renderer );

	// destroys memory allocations
	void reset();

	/// map uniform buffers so that they can be written to.
	/// \return an address into gpu readable memory
	/// also resets indices into internal matrix state structures
	void begin( size_t frame_ );

	// unmap uniform buffers 
	void end();

	// write current descriptor buffer state to GPU buffer
	// updates descriptorOffsets - saves these in frameShadow
	void flushUniformBufferState();

	void bindDescriptorSets( const VkCommandBuffer & cmd );
	void bindPipeline( const VkCommandBuffer& cmd );


	// return the one buffer which is used for all dynamic buffer memory within this context.
	const VkBuffer& getVkBuffer() const;

	// get dynamic offsets for all descriptorsets which are currently bound
	const std::vector<uint32_t>& getDynamicUniformBufferOffsets() const;

	// return a vector of descriptorsets which are currently bound
	// in order of the current pipeline's descriptorSetLayout.
	// std::vector<VkDescriptorSet> getBoundDescriptorSets();

	// get descriptorSetLayout for a shader
	std::vector<VkDescriptorSetLayout> getDescriptorSetLayoutForShader( const shared_ptr<of::vk::Shader>& shader_ );

	// lazily store uniform data into local CPU memory
	template<typename UniformT>
	bool setUniform( const std::string& name_, const UniformT & pSource );

	// fetch uniform
	template<typename UniformT>
	UniformT & getUniform( const std::string & name_ );

	// fetch const uniform
	template<typename UniformT>
	const UniformT & getUniform( const std::string & name_ ) const;

	inline const glm::mat4x4 & getViewMatrix()       const { return getUniform<glm::mat4x4>( "viewMatrix"       ); }
	inline const glm::mat4x4 & getModelMatrix()      const { return getUniform<glm::mat4x4>( "modelMatrix"      ); }
	inline const glm::mat4x4 & getProjectionMatrix() const { return getUniform<glm::mat4x4>( "projectionMatrix" ); }

	void setViewMatrix( const glm::mat4x4& mat_ );
	void setProjectionMatrix( const glm::mat4x4& mat_ );

	void translate(const glm::vec3& v_);
	void rotateRad( const float & degrees_, const glm::vec3& axis_ );

	// push local ubo uniform group state
	void pushBuffer( const std::string& ubo_ );
	
	// pop local ubo uniform group state
	void popBuffer( const std::string& ubo_ );

	// push currentMatrix state
	void pushMatrix(){
		pushBuffer( "DefaultMatrices" );
	}
	// pop current Matrix state
	void popMatrix(){
		popBuffer( "DefaultMatrices" );
	}

	// store vertex and index data inside the current dynamic memory frame
	// return memory mapping offets based on current memory buffer.
	bool storeMesh( const ofMesh& mesh_, std::vector<VkDeviceSize>& vertexOffsets, std::vector<VkDeviceSize>& indexOffsets );
	
	
	private:
		friend class Shader;
		// only shader should need to access this.
		void storeDescriptorSetLayout( of::vk::Shader::SetLayout && setLayout_ );

};

// ----------------------------------------------------------------------

template<typename UniformT>
inline bool Context::setUniform( const std::string & name_, const UniformT & uniform_ ){
	auto uboIt = mCurrentFrameState.mUniformMembers.find( name_ );
	if ( uboIt == mCurrentFrameState.mUniformMembers.end() ){
		ofLogWarning() << "Cannot set uniform: '" << name_ << "' - Not found in shader.";
		return false;
	}
	auto & ubo = uboIt->second;
	if ( sizeof( UniformT ) != ubo.range ){
		// assignment would overshoot - possibly wrong type for assignment : refuse assignment.
		ofLogWarning() << "Cannot assign to uniform: '" << name_ << "' - data size is incorrect: " << sizeof( UniformT ) << " Byte, expected: " << ubo.range << "Byte";
		return false;
	}
	UniformT& uniform = reinterpret_cast<UniformT&>( ( ubo.buffer->state.data[ubo.offset] ) );
	uniform = uniform_;
	ubo.buffer->state.stackId = -1; // mark dirty
	return true;
};

// ----------------------------------------------------------------------

template<typename UniformT>
inline UniformT& Context::getUniform( const std::string & name_ ){
	static UniformT errUniform;
	auto uboIt = mCurrentFrameState.mUniformMembers.find( name_ );
	if ( uboIt == mCurrentFrameState.mUniformMembers.end() ){
		ofLogWarning() << "Cannot get uniform: '" << name_ << "' - Not found in shader.";
		return errUniform;
	}
	auto & ubo = uboIt->second;
	if ( sizeof( UniformT ) != ubo.range ){
		// assignment would overshoot - return a default uniform.
		ofLogWarning() << "Cannot get uniform: '" << name_ << "' - data size is incorrect: " << sizeof( UniformT ) << " Byte, expected: " << ubo.range << "Byte";
		return errUniform;
	}
	UniformT& uniform = reinterpret_cast<UniformT&>( ( ubo.buffer->state.data[ubo.offset] ) );
	ubo.buffer->state.stackId = -1; // mark dirty
	return uniform;
};

// ----------------------------------------------------------------------

template<typename UniformT>
inline const UniformT& Context::getUniform( const std::string & name_ ) const {
	static const UniformT errUniform;
	auto uboIt = mCurrentFrameState.mUniformMembers.find( name_ );
	if ( uboIt == mCurrentFrameState.mUniformMembers.end() ){
		ofLogWarning() << "Cannot get uniform: '" << name_ << "' - Not found in shader.";
		return errUniform;
	}
	auto & ubo = uboIt->second;
	if ( sizeof( UniformT ) != ubo.range ){
		// assignment would overshoot - return a default uniform.
		ofLogWarning() << "Cannot get uniform: '" << name_ << "' - data size is incorrect: " << sizeof( UniformT ) << " Byte, expected: " << ubo.range << "Byte";
		return errUniform;
	}
	const UniformT& uniform = reinterpret_cast<UniformT&>( ( ubo.buffer->state.data[ubo.offset] ) );
	return uniform;
};

//// ----------------------------------------------------------------------
//
//inline std::vector<VkDescriptorSet> of::vk::Context::getBoundDescriptorSets(){
//	// return a vector of descriptorsets which are currently bound
//	// in order of the current pipeline's descriptorSetLayout.
//	std::vector<VkDescriptorSet> ret;
//	const auto & keyVec = mCurrentShader->getSetLayoutKeys();
//	ret.reserve( keyVec.size() );
//	for ( auto & key : keyVec ){
//		// Caution: we just assume there is a descriptorset 0!
//		ret.push_back( mDescriptorSets[key] );
//	}
//	return ret;
//}
//
//// ----------------------------------------------------------------------
//
inline std::vector<VkDescriptorSetLayout> of::vk::Context::getDescriptorSetLayoutForShader( const shared_ptr<of::vk::Shader>& shader_ ){
	// get descriptorSetLayout for a shader
	std::vector<VkDescriptorSetLayout> ret;
	auto & layoutKeys = shader_->getSetLayoutKeys();
	if ( !layoutKeys.empty() ){
		// Caution: we just assume there is a descriptorset 0!
		for ( const auto&layoutKey : layoutKeys ){
			ret.push_back( mDescriptorSetLayouts[layoutKey]->vkDescriptorSetLayout );
		}
	}
	return ret;
}

} // namespace vk
} // namespace of
