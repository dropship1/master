#ifndef BRIGHT_AUDIO_SOUNDMANAGER_H
#define BRIGHT_AUDIO_SOUNDMANAGER_H


#include <cstdlib>
#include <iostream>
#include <openal-1.1/al.h>
#include <openal-1.1/alc.h>
#include <map>
#include <string>


namespace bright{

  namespace audio{


class SoundManager{
public:
  SoundManager(){};
  ~SoundManager(){};
  int initialize();
  //int create_wav_source(const std::string &name, const std::string &filename);
  int create_wav_source(const std::string &name, std::istream& stream);

  void set_listener_position(ALfloat x, ALfloat y, ALfloat z);
  void set_listener_direction(ALfloat x, ALfloat y, ALfloat z);
  void set_listener_velocity(ALfloat x, ALfloat y, ALfloat z);

  void set_source_position(const std::string &name, ALfloat x, ALfloat y, ALfloat z);
  void set_source_direction(const std::string &name, ALfloat x, ALfloat y, ALfloat z);
  void set_source_velocity(const std::string &name, ALfloat x, ALfloat y, ALfloat z);

  int play_source(const std::string &name);

  void cleanup();

private:
  std::map<std::string, ALuint *> sources;
  std::map<std::string, ALuint *> buffers;
  ALCdevice *device;                                     //Create an OpenAL Device
  ALCcontext *context;                                   //And an OpenAL Context

};


  }
}

#endif