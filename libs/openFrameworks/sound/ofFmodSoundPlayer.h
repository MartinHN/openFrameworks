#pragma once

#include "ofConstants.h"


#include "ofBaseSoundPlayer.h"
#define FMOD_CHANNELS 50

extern "C" {
#include "fmod.h"
#include "fmod_errors.h"
    
    

}

#include "ofEvents.h"

//		TO DO :
//		---------------------------
// 		-fft via fmod, as in the last time...
// 		-close fmod if it's up
//		-loadSoundForStreaming(char * fileName);
//		---------------------------

// 		interesting:
//		http://www.compuphase.com/mp3/mp3loops.htm


// ---------------------------------------------------------------------------- SOUND SYSTEM FMOD

// --------------------- global functions:
void ofFmodSoundStopAll();
void ofFmodSoundSetVolume(float vol);
void ofFmodSoundUpdate();						// calls FMOD update.
float * ofFmodSoundGetSpectrum(int nBands);		// max 512...


// --------------------- player functions:
class ofFmodSoundPlayer : public ofBaseSoundPlayer {

	public:

		ofFmodSoundPlayer();
		virtual ~ofFmodSoundPlayer();

		bool loadSound(string fileName, bool stream = false);
		void unloadSound();
		void play();
		void stop();

		void setVolume(float vol);
		void setPan(float vol);
		void setSpeed(float spd);
		void setPaused(bool bP);
		void setLoop(bool bLp);
		void setMultiPlay(bool bMp);
		void setPosition(float pct); // 0 = start, 1 = end;
		void setPositionMS(int ms);

		float getPosition();
		int getPositionMS();
		bool getIsPlaying();
		float getSpeed();
		float getPan();
		float getVolume();
		bool isLoaded();

		static void initializeFmod();
		static void closeFmod();

		bool isStreaming;
		bool bMultiPlay;
		bool bLoop;
		bool bLoadedOk;
		bool bPaused;
		float pan; // -1 to 1
		float volume; // 0 - 1
		float internalFreq; // 44100 ?
		float speed; // -n to n, 1 = normal, -1 backwards
		unsigned int length; // in samples;
    
		FMOD_RESULT result;
		FMOD_CHANNEL * channel;
		FMOD_SOUND * sound;
    
    
    void setStopMS(float ms);
    void setStopEvent(ofEvent<bool> & ev);
    static std::map<FMOD_CHANNEL *,ofEvent<bool>* > evDic;

    
};


extern "C"{
    FMOD_RESULT F_CALLBACK stopCallback(FMOD_CHANNEL *channel, FMOD_CHANNEL_CALLBACKTYPE type, void *commanddata1, void *commanddata2){
        for(std::map<FMOD_CHANNEL *,ofEvent<bool>* >::iterator it = ofFmodSoundPlayer::evDic.begin(); it != ofFmodSoundPlayer::evDic.end();++it){
            if(it->first == channel){
                bool t = true;
                ofNotifyEvent(*it->second, t, channel);
                ofFmodSoundPlayer::evDic.erase(it->first);
                return FMOD_OK;
            }
        }
        return FMOD_ERR_ALREADYLOCKED;
    };
}
