#pragma once
#include <string>
#include <map>
#include "vulkan/vulkan.h"
#include "vk/spirv-cross/include/spirv_cross.hpp"


//   The smallest unit we may bind in Vulkan are DescriptorSets.
//   
//   Each set has its own namespace for bindings. Bindings may be sparse.
//   
//   If no set is specified explicitly in the shader, set 0 is used.
//   
//   A DescriptorSet is described by a DescriptorSetLayout.
//   
//   A DescriptorSetLayout is a table of DescriptorSetLayoutBindings
//   
//   A DescriptorSetLayoutBinding describes the type of descriptor and its count-
//   if the count is >1, this DSLB represents an
//   array of descriptors of the same type.
//   
//   An ordered array of DescriptorSetLayout is used to describe a PipelineLayout,
//   it shows which DescriptorSets in sequence describe the inputs for a pipeline.
//   
//   
//   set 0 \
//   |- binding 0 - descriptor 0
//   |- binding 1 - decriptors[] 1,2,3
//   |- <empty>
//   |- binding 3 - descriptor 4
//   
//   set 1 \
//   |- binding 0
//   |- binding 1
//   
//   There does not seem to be a maximum number of descriptors that we can allocate,
//   it is limited by GPU memory. On AMD/Radeon, a descriptor consumes at most 32 bytes,
//   so you can have 32k descriptors per MB of GPU memory. On AMD, there is a privileged
//   256 MB of memory that is directly accessible through the CPU - on NVIDIA, this
//   chunk is possibly larger. This is where Descriptors are stored.
//   
//   Descriptors are allocated from DescriptorPools. If you use
//   VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT, this means the pool uses a dynamic
//   allocator, which brings the risk of fragmentation. If you set up the pool so that it
//   only supports RESET, then this pool will have a linear allocator, memory will be pre-
//   allocated in a big chunk, and allocating and resetting is effectively updating an
//   offset, similar to what of::vk::Allocator does.
//   
//   Also, vkWriteDescriptorSet writes directly to GPU memory.
//   
//   There is a maximum number of DescriptorSets and Descriptors that can be bound
//   at the same time.
//   
//   Here is some more information about the Vulkan binding model:
//   https://developer.nvidia.com/vulkan-shader-resource-binding
//   
//   Some information on Descriptor Sets and fast paths
//   http://gpuopen.com/wp-content/uploads/2016/03/VulkanFastPaths.pdf
//   
//   
//   
//   
//	 Here is a diagram on how vertex attribute binding works, based on vkspec 1.0, pp.389
//
//
//     SHADER                                                       | Pipeline setup                               PIPELINE              |  COMMAND BUFFER   | BUFFERS (GPU MEMORY)
//   ---------------------------------------------------------------|--------------------------------------------|-----------------------|-------------------|---------------------
//     shader variable  => associated with => input attribute number| association on per-pipeline basis          | vertex input bindings |  association on a per-draw basis
//                                            in GLSL, set through  | when defining the pipeline with            |                       |  when binding buffers using
//                                           "location" qualifier   | "VkPipelineVertexInputStateCreateInfo"     | "binding number"      |  "vkCmdBindVertexBuffers"
//                                                                  |                                            |                       |                   |
//                                                                  | attributeDescription -> bindingDescription |                       |                   |
//                                                                  |    "location"        -> "binding"          |                       |                   |
//    vec3  inPos                   =>        (location = 0)        | --------------------.  .------------------ | [0] ----------------- |  ------.  .------ |   buffer A
//                                                                  |                      \/                    |                       |         \/        |
//                                                                  |                      /\                    |                       |         /\        |
//    vec3  inNormal                =>        (location = 1)        | --------------------'  '------------------ | [1] ----------------- |  ------'  '------ |   buffer B
//


namespace of{
namespace vk{

class Context;

class Shader
{
public: 
	struct BufferRange
	{
		size_t offset;
		size_t range;
	};

	struct BindingInfo
	{
		uint32_t set;                                     // Descriptor set this binding belongs to
		VkDescriptorSetLayoutBinding binding;             // Describes mapping of binding number to descriptors, 
		                                                  // number and type of descriptors under a binding.
		uint32_t size;                                    // size in bytes (corresponds to struct size of the UBO)
		                                                  // which has members with names ("projectionMatrix", "viewMatrix", ...) 
		std::string name;                                 // Uniform Buffer Block name from shader
		std::map< std::string, BufferRange> memberRanges; // Memory offsets for members, indexed by name
	};

	// layout for a descriptorSet
	// use this to create vkDescriptorSetLayout
	struct SetLayout
	{
		std::vector<BindingInfo> bindings; // must be in ascending order, but may be sparse
		uint64_t key;
		void calculateHash();
	};

private:

	VkDevice mDevice = nullptr;
	Context* mContext;
	shared_ptr<VkPipelineLayout> mPipelineLayout; // context sets this.

	std::map<VkShaderStageFlagBits, VkShaderModule>         mModules;
	std::vector<VkPipelineShaderStageCreateInfo>	        mStages;

	std::map<VkShaderStageFlagBits, std::shared_ptr<spirv_cross::Compiler>> mCompilers;
	
	// map from shader uniform name to uniform binding 
	// when we say "uniform" this may be any of VkDescriptorType, so: UBOs, samplers ...
	// Binding info also contains set number for a binding.
	std::map<std::string, BindingInfo> mUniforms;

	//// sequence of setLayouts forming the pipelineLayout for this shader
	//std::vector<SetLayout> mSetLayouts;
	
	// Sequence of hashes of SetLayouts - which reference vkDescriptorSetLayouts in Context.
	// This describes the sequence for the pipeline layout for this shader.
	std::vector<uint64_t>  mDescriptorSetLayoutKeys;  

	// ----------------------------------------------------------------------
	// Derive bindings from shader reflection using SPIR-V Cross.
	// we want to extract as much information out of the shader metadata as possible
	// all this data helps us to create descriptors, and also to create layouts fit
	// for our pipelines.
	void reflect();

	void buildSetLayouts();

	void processVertexInputs( spirv_cross::ShaderResources &shaderResources, spirv_cross::Compiler & compiler );

	void processResource( spirv_cross::Compiler & compiler, spirv_cross::Resource & ubo, const VkShaderStageFlagBits & shaderStage );

public:

	const struct Settings
	{
		of::vk::Context* context;
		std::map<VkShaderStageFlagBits, std::string> sources;
	} mSettings;

	struct VertexInfo
	{
		std::vector<VkVertexInputBindingDescription>   bindingDescription;	  // describes data input parameters for pipeline slots
		std::vector<VkVertexInputAttributeDescription> attribute;	          // mapping of attribute locations to pipeline slots
		VkPipelineVertexInputStateCreateInfo vi;
	} mVertexInfo;

	// ----------------------------------------------------------------------

	// shader object needs to be initialised based on spir-v sources to be useful
	Shader( const Settings& settings_ );

	// ----------------------------------------------------------------------

	~Shader()
	{
		mCompilers.clear();
		// reset shader object
		for ( auto &s : mModules ){
			if ( s.second != nullptr ){
				vkDestroyShaderModule( mDevice, s.second, nullptr );
				s.second = nullptr;
			}
		}
		mModules.clear();
		mStages.clear();
	}

	// ----------------------------------------------------------------------

	const std::vector<uint64_t>& getSetLayoutKeys() const{
		return mDescriptorSetLayoutKeys;
	}

	// ----------------------------------------------------------------------

	// return vector create info for all shader modules which compiled successfully
	const std::vector<VkPipelineShaderStageCreateInfo>& getShaderStageCreateInfo(){
		return mStages;
	}

	// ----------------------------------------------------------------------
	// return vertex input state create info
	// this hold the layout and wiring of attribute inputs to vertex bindings
	const VkPipelineVertexInputStateCreateInfo& getVertexInputState(){
		return mVertexInfo.vi;
	}
	
	// ----------------------------------------------------------------------
	// context may set pipeline layout for this shader.
	void setPipelineLayout( const std::shared_ptr<VkPipelineLayout>& pipelineLayout ){
		mPipelineLayout = pipelineLayout;
	}

	// ----------------------------------------------------------------------
	const std::shared_ptr<VkPipelineLayout>& getPipelineLayout() const{
		return mPipelineLayout;
	}

};

} // namespace vk
} // namespace of
