#include "audio/soundmanager.hpp"

using namespace bright::audio;


int SoundManager::initialize(){

  //Clear error code
  alGetError(); 
  device = alcOpenDevice(NULL);

  //ERROR NO SOUND DEVICE
  if(!device){
    //Error during device opening
    return 1;    
  }

  context = alcCreateContext(device, NULL);
  alcMakeContextCurrent(context);

  //ERROR NO SOUND CONTEXT
  if(!context){
    std::cout << "ERROR DURING CONTEXT HANDLING"   << std::endl;
    return 1;  //Error during context handeling
  }

  //Sound setting variables
  //Position of the source sound
  ALfloat SourcePos[] = { 0.0, 0.0, 0.0 };                                    
  //Velocity of the source sound
  ALfloat SourceVel[] = { 0.0, 0.0, 0.0 };                                    
  //Position of the listener
  ALfloat ListenerPos[] = { 0.0, 0.0, 0.0 };                                  
  //Velocity of the listener
  ALfloat ListenerVel[] = { 0.0, 0.0, 0.0 };                                  
  //Orientation of the listener
  ALfloat ListenerOri[] = { 0.0, 0.0, -1.0,  0.0, 1.0, 0.0 };                 
  //First direction vector, then vector pointing up) 
                                                                              
  //Listener                                                                  
  //Set position of the listener             
  alListenerfv(AL_POSITION,    ListenerPos);                                 
  //Set velocity of the listener 
  alListenerfv(AL_VELOCITY,    ListenerVel);                                 
  //Set orientation of the listener
  alListenerfv(AL_ORIENTATION, ListenerOri);                                  

  //SUCCESS
  return 0; 
}

int SoundManager::create_wav_source(const std::string &name, std::istream& stream){
  ALuint *source = new ALuint();
  ALuint *buffer = new ALuint();

  //Variables to store info about the WAVE file (all of them is not needed for OpenAL)
  char type[4];
  unsigned long size,chunkSize;
  short formatType,channels;
  unsigned long sampleRate,avgBytesPerSec;
  short bytesPerSample,bitsPerSample;
  unsigned long dataSize;
    
  //Check that the WAVE file is OK
  //Reads the first bytes in the file
  stream.read(type, 4);                                              
  //Should be "RIFF"
  if(type[0]!='R' || type[1]!='I' || type[2]!='F' || type[3]!='F'){  
    std::cout << "ERROR NO RIFF" << std::endl;
    return 1;
  }

  //Continue to read the file
  stream.read((char*)&size, sizeof(unsigned long));
  //Continue to read the file
  stream.read(type, 4);
  //This part should be "WAVE"
  if (type[0]!='W' || type[1]!='A' || type[2]!='V' || type[3]!='E'){           
    std::cout << "ERROR NOT WAVE" << std::endl;
    return 1;
  }

    
  //Continue to read the file
  stream.read(type, 4);
  //This part should be "fmt "
  if (type[0]!='f' || type[1]!='m' || type[2]!='t' || type[3]!=' '){          
    std::cout << "ERROR NOT FMT" << std::endl;
    return 1;
  }

    
  //Now we know that the file is a acceptable WAVE file
  //Info about the WAVE data is now read and stored
  stream.read((char*)&chunkSize, sizeof(unsigned long));
  stream.read((char*)&formatType, sizeof(short));
  stream.read((char*)&channels, sizeof(short));
  stream.read((char*)&sampleRate, sizeof(unsigned long));
  stream.read((char*)&avgBytesPerSec, sizeof(unsigned long));
  stream.read((char*)&bytesPerSample, sizeof(short));
  stream.read((char*)&bitsPerSample, sizeof(short));
  
  stream.read(type, 4);
  //This part should be "data"
  if (type[0]!='d' || type[1]!='a' || type[2]!='t' || type[3]!='a'){           
    std::cout << "ERROR MISSING DATA" << std::endl;
    return 1;
  }
    
  //The size of the sound data is read
  stream.read((char*)&dataSize, sizeof(unsigned long));
      
  //Allocate memory for the sound data
  unsigned char* buf= new unsigned char[dataSize];                            
  //Read the sound data
  stream.read((char*)buf, dataSize);

  //The Sample Rate of the WAVE file
  ALuint frequency=sampleRate;                                               
  //The audio format (bits per sample, number of channels)
  ALenum format=0;                                                            
  
  //Generate one OpenAL Buffer and link to "buffer"
  alGenBuffers(1, buffer);                                                    
  //Generate one OpenAL Source and link to "source"
  alGenSources(1, source);                                                   
  if(alGetError() != AL_NO_ERROR){
    std::cout << "ERROR GENSOURCE" << std::endl;
    //Error during buffer/source generation
    return 1; 
  }   

  //Figure out the format of the WAVE file   
  if(bitsPerSample == 8){
    if(channels == 1)
      format = AL_FORMAT_MONO8;
    else if(channels == 2)
      format = AL_FORMAT_STEREO8;
  }
  else if(bitsPerSample == 16){
    if(channels == 1)
      format = AL_FORMAT_MONO16;
    else if(channels == 2)
      format = AL_FORMAT_STEREO16;
  }
  if(!format){
   std::cout << "ERROR: Wrong BitPerSample" << std::endl;
   return 1;
  }

  //Store the sound data in the OpenAL Buffer
  alBufferData(*buffer, format, buf, dataSize, frequency);                   
  if(alGetError() != AL_NO_ERROR){
    std::cout << "ERROR LOADING ALBUFFER" << std::endl;
    //Error during buffer loading
    return 1;
  }                         
  
  //Sound setting variables
  //Position of the source sound
  ALfloat SourcePos[] = { 0.0, 0.0, 0.0 };       
  //Velocity of the source sound                             
  ALfloat SourceVel[] = { 0.0, 0.0, 0.0 };                                   
  
  //Source
  //Link the buffer to the source
  alSourcei (*source, AL_BUFFER,   *buffer);                                   
  //Set the pitch of the source
  alSourcef (*source, AL_PITCH,    1.0f     );                                 
  //Set the gain of the source
  alSourcef (*source, AL_GAIN,     1.0f     );                                 
  //Set the position of the source
  alSourcefv(*source, AL_POSITION, SourcePos);                                 
  //Set the velocity of the source
  alSourcefv(*source, AL_VELOCITY, SourceVel);                                 
  //Set if source is looping sound
  alSourcei (*source, AL_LOOPING,  AL_FALSE );                                 

  sources[name] = source;
  buffers[name] = buffer;

  return 0;
}


void SoundManager::set_listener_position(ALfloat x, ALfloat y, ALfloat z){
  alListener3f(AL_POSITION, x, y, z);
}


void SoundManager::set_listener_direction(ALfloat x, ALfloat y, ALfloat z){
  alListener3f(AL_DIRECTION, x, y, z);
}


void SoundManager::set_listener_velocity(ALfloat x, ALfloat y, ALfloat z){
  alListener3f(AL_VELOCITY, x, y, z);
}


void SoundManager::set_source_position(const std::string &name, ALfloat x, ALfloat y, ALfloat z){
  alSource3f(*(sources.find(name)->second), AL_POSITION, x, y, z);
}


void SoundManager::set_source_direction(const std::string &name, ALfloat x, ALfloat y, ALfloat z){
  alSource3f(*(sources.find(name)->second), AL_DIRECTION, x, y, z);
}


void SoundManager::set_source_velocity(const std::string &name, ALfloat x, ALfloat y, ALfloat z){
  alSource3f(*(sources.find(name)->second), AL_VELOCITY, x, y, z);
}


int SoundManager::play_source(const std::string &name){
  ALenum error = alGetError();
  ALuint *source = sources.find(name)->second;
  alSourcePlay( *source );
  error = alGetError();
  if(error != AL_NO_ERROR){
    std::cout << "ERROR PLAYING SOUND" << std::endl;
    return 1;
  }  
  return 0;
}


void SoundManager::cleanup(){
  //Clean-up

  for(auto const &name : sources) {
    ALuint *source = name.second;
    //Delete the OpenAL Source
    alDeleteSources(1, source); 
  }
  for(auto const &name : buffers) {
    ALuint *buffer = name.second;
    //Delete the OpenAL Buffer
    alDeleteBuffers(1, buffer); 
  }

  //Make no context current
  alcMakeContextCurrent(NULL);                                              
  //Destroy the OpenAL Context
  alcDestroyContext(context);                                              
  //Close the OpenAL Device 
  alcCloseDevice(device);                                                  
}