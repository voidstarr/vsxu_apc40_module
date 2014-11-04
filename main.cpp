/**
* Project: VSXu: Realtime modular visual programming engine.
*
* This file is part of Vovoid VSXu.
*
* @author Jonatan Wallmander, Robert Wenzel, Vovoid Media Technologies AB Copyright (C) 2003-2013
* @see The GNU Lesser General Public License (LGPL)
*
* VSXu Engine is free software; you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
* or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
* for more details.
*
* You should have received a copy of the GNU Lesser General Public License along
* with this program; if not, write to the Free Software Foundation, Inc.,
* 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/

#include "vsx_param.h"
#include "vsx_module.h"
#include "vsxfst.h"
#include "RtMidi/RtMidi.h"
#include <sstream>

#define CC_ABSOLUTE 0
#define CC_ID_TRACK 7
#define CC_ID_MASTER 14
#define CC_ID_KNOB 47
#define NOTE_ON 1
#define NOTE_OFF 2
#define ON 1.0f
#define OFF 0.0f

class vsx_apc_fourty : vsx_module {
    unsigned int current_port=0;
    unsigned int port_count=0;
    RtMidiIn* m_midi_in;
    vsx_module_param_int* midi_source;
    vsx_module_param_float* clip_launch[9][5];
    vsx_module_param_float* scene_launch[5];
    vsx_module_param_float* clip_stop[8];
    vsx_module_param_float* track_select[8];
    vsx_module_param_float* activator[8];
    vsx_module_param_float* solo[8];
    vsx_module_param_float* record_arm[8];
    vsx_module_param_float* fader[10];
    vsx_module_param_float* shift;
    vsx_module_param_float* knob;

public:

    vsx_apc_fourty()
    {
        try {
            m_midi_in = new RtMidiIn(RtMidi::UNSPECIFIED,"vsxu_midi",1024*6);
            m_midi_in->ignoreTypes(false,false,false);
          }
          catch (RtError &error) {
            error.printMessage();
          }
    }

    ~vsx_apc_fourty()
    {
       delete m_midi_in;
    }


    void module_info(vsx_module_info* info)
    {
        std::stringstream in_buff;
        in_buff << "midi_source:enum?";
        port_count = m_midi_in->getPortCount();
        for(int i=0;i<port_count;i++)
        {
            in_buff << i << "__" << remove_spaces(m_midi_in->getPortName(i)).c_str();
            if(i<port_count-1)
                in_buff << "|";
        }
        in_buff << "";
      info->in_param_spec = in_buff.str().c_str();
      info->identifier = "sound;midi;akai_apc40";
      info->out_param_spec =
              "tracks:complex{"
                "track1:complex{"
                    "clip_launch1_1:float,"
                    "clip_launch1_2:float,"
                    "clip_launch1_3:float,"
                    "clip_launch1_4:float,"
                    "clip_launch1_5:float,"
                    "clip_stop1:float,"
                    "track_select1:float,"
                    "activator1:float,"
                    "solo1:float,"
                    "record_arm1:float,"
                    "fader1:float"
                "},"
              "track2:complex{"
                  "clip_launch2_1:float,"
                  "clip_launch2_2:float,"
                  "clip_launch2_3:float,"
                  "clip_launch2_4:float,"
                  "clip_launch2_5:float,"
                  "clip_stop2:float,"
                  "track_select2:float,"
                  "activator2:float,"
                  "solo2:float,"
                  "record_arm2:float,"
                  "fader2:float"
              "},"
              "track3:complex{"
                  "clip_launch3_1:float,"
                  "clip_launch3_2:float,"
                  "clip_launch3_3:float,"
                  "clip_launch3_4:float,"
                  "clip_launch3_5:float,"
                  "clip_stop3:float,"
                  "track_select3:float,"
                  "activator3:float,"
                  "solo3:float,"
                  "record_arm3:float,"
                  "fader3:float"
              "},"
              "track4:complex{"
                  "clip_launch4_1:float,"
                  "clip_launch4_2:float,"
                  "clip_launch4_3:float,"
                  "clip_launch4_4:float,"
                  "clip_launch4_5:float,"
                  "clip_stop4:float,"
                  "track_select4:float,"
                  "activator4:float,"
                  "solo4:float,"
                  "record_arm4:float,"
                  "fader4:float"
              "},"
              "track5:complex{"
                  "clip_launch5_1:float,"
                  "clip_launch5_2:float,"
                  "clip_launch5_3:float,"
                  "clip_launch5_4:float,"
                  "clip_launch5_5:float,"
                  "clip_stop5:float,"
                  "track_select5:float,"
                  "activator5:float,"
                  "solo5:float,"
                  "record_arm5:float,"
                  "fader5:float"
              "},"
              "track6:complex{"
                  "clip_launch6_1:float,"
                  "clip_launch6_2:float,"
                  "clip_launch6_3:float,"
                  "clip_launch6_4:float,"
                  "clip_launch6_5:float,"
                  "clip_stop6:float,"
                  "track_select6:float,"
                  "activator6:float,"
                  "solo6:float,"
                  "record_arm6:float,"
                  "fader6:float"
              "},"
              "track7:complex{"
                  "clip_launch7_1:float,"
                  "clip_launch7_2:float,"
                  "clip_launch7_3:float,"
                  "clip_launch7_4:float,"
                  "clip_launch7_5:float,"
                  "clip_stop7:float,"
                  "track_select7:float,"
                  "activator7:float,"
                  "solo7:float,"
                  "record_arm7:float,"
                    "fader7:float"
              "},"
              "track8:complex{"
                  "clip_launch8_1:float,"
                  "clip_launch8_2:float,"
                  "clip_launch8_3:float,"
                  "clip_launch8_4:float,"
                  "clip_launch8_5:float,"
                  "track_select8:float,"
                  "clip_stop8:float,"
                  "activator8:float,"
                  "solo8:float,"
                  "record_arm8:float,"
                  "fader8:float"
              "},"
              "track9:complex{"
                "scene_launch1:float,"
                "scene_launch2:float,"
                "scene_launch3:float,"
                "scene_launch4:float,"
                "scene_launch5:float,"
                "shift:float,"
                "knob_dir:float,"
                "fader9:float"
              "}"
             "}";
      info->component_class = "system";
      info->description = "This represents the Akai APC40 midi\ncontroller.";
    }

    void declare_params(vsx_module_param_list& in_parameters, vsx_module_param_list& out_parameters)
    {
        std::stringstream intbuf;
        for(int i=1;i<10;i++)
        {
            intbuf << "fader" << i;
            fader[i-1]=(vsx_module_param_float*)out_parameters.create(VSX_MODULE_PARAM_ID_FLOAT,intbuf.str().c_str());
            fader[i-1]->set(0.0f);
            intbuf.str("");
           if(i==9){
               for(int j=1;j<6;j++)
                {
                   intbuf << "scene_launch" << j;
                   scene_launch[j-1] = (vsx_module_param_float*)out_parameters.create(VSX_MODULE_PARAM_ID_FLOAT,intbuf.str().c_str());
                   scene_launch[j-1]->set(0.0f);
                   intbuf.str("");
                }
               shift = (vsx_module_param_float*)out_parameters.create(VSX_MODULE_PARAM_ID_FLOAT,"shift");
               knob = (vsx_module_param_float*)out_parameters.create(VSX_MODULE_PARAM_ID_FLOAT,"knob_dir");
               shift->set(0.0);
               knob->set(0.0);
               continue;
           }
           for(int j=1;j<6;j++)
            {
               intbuf << "clip_launch" << i;
               intbuf << "_";
               intbuf << j;
               clip_launch[i-1][j-1] = (vsx_module_param_float*)out_parameters.create(VSX_MODULE_PARAM_ID_FLOAT,intbuf.str().c_str());
               clip_launch[i-1][j-1]->set(0.0f);
               intbuf.str("");
            }
           intbuf.str("");
           intbuf << "track_select" << i;
           track_select[i-1] = (vsx_module_param_float*)out_parameters.create(VSX_MODULE_PARAM_ID_FLOAT,intbuf.str().c_str());
           track_select[i-1]->set(0.0f);
           intbuf.str("");
           intbuf << "clip_stop" << i;
           clip_stop[i-1] = (vsx_module_param_float*)out_parameters.create(VSX_MODULE_PARAM_ID_FLOAT,intbuf.str().c_str());
           clip_stop[i-1]->set(0.0f);
           intbuf.str("");
           intbuf << "activator" << i;
           activator[i-1] = (vsx_module_param_float*)out_parameters.create(VSX_MODULE_PARAM_ID_FLOAT,intbuf.str().c_str());
           activator[i-1]->set(0.0f);
           intbuf.str("");
           intbuf << "solo" << i;
           solo[i-1] = (vsx_module_param_float*)out_parameters.create(VSX_MODULE_PARAM_ID_FLOAT,intbuf.str().c_str());
           solo[i-1]->set(0.0f);
           intbuf.str("");
           intbuf << "record_arm" << i;
           record_arm[i-1] = (vsx_module_param_float*)out_parameters.create(VSX_MODULE_PARAM_ID_FLOAT,intbuf.str().c_str());
           record_arm[i-1]->set(0.0f);
           intbuf.str("");
        }
        redeclare_in_params(in_parameters);
        m_midi_in->openPort(current_port);

      loading_done = true;
    }

    void redeclare_in_params(vsx_module_param_list& in_param)
    {
        midi_source = (vsx_module_param_int*)in_param.create(VSX_MODULE_PARAM_ID_FLOAT,"midi_source");
        midi_source->set(0);
    }

    void run() {
        if(current_port!=get_port())
        {
            int new_src = midi_source->get();
            if (new_src > port_count-1)
            {
                new_src = port_count-1;
            }
            try {
                current_port=new_src;
                m_midi_in->closePort();
               m_midi_in->openPort(current_port);
              }
              catch (RtError &error) {
                message = error.getMessage().c_str();
              }
        } else if (port_count!=m_midi_in->getPortCount()){
            port_count = m_midi_in->getPortCount();
            redeclare_in = true;
        }
        handle_messages();

    }

    void handle_messages()
    {
        std::vector<unsigned char> mess;
        m_midi_in->getMessage(&mess);
        if(mess.size() > 0)
        {
            int byte1,byte2,byte3,chan,type;
            byte1=mess.at(0);
            byte2=mess.at(1);
            byte3=mess.at(2);
            if(byte1-0xB0>=0)
            {
               chan = byte1-0xB0;
               type = CC_ABSOLUTE;
            } else if (byte1-0x90>=0)
            {
               chan = byte1-0x90;
               type = NOTE_ON;
            } else if (byte1-0x80>=0)
            {
               chan = byte1-0x80;
               type = NOTE_OFF;
            }
            switch(type){
                case NOTE_ON:
                    vsx_printf("NOTE_ON: %X\n", byte2);
                    fflush(stdout);
                    switch(byte2)
                    {
                        case 48:
                            record_arm[chan]->set(ON);
                        break;
                        case 49:
                             solo[chan]->set(ON);
                        break;
                        case 50:
                            activator[chan]->set(ON);
                        break;
                        case 51:
                            track_select[chan]->set(ON);
                        break;
                        case 52:
                            clip_stop[chan]->set(ON);
                        break;
                        case 53:
                        case 54:
                        case 55:
                        case 56:
                        case 57:
                            clip_launch[chan][byte2 - 53]->set(ON);
                        break;
                        case 81:
                            shift->set(ON);
                        break;
                        case 82:
                        case 83:
                        case 84:
                        case 85:
                        case 86:
                        scene_launch[byte2-82]->set(ON);
                        break;
                    }
                break;
            case NOTE_OFF:
                vsx_printf("NOTE_OFF: %X\n", byte2);
                fflush(stdout);
                switch(byte2)
                {
                    case 48:
                        record_arm[chan]->set(OFF);
                    break;
                    case 49:
                         solo[chan]->set(OFF);
                    break;
                    case 50:
                        activator[chan]->set(OFF);
                    break;
                    case 51:
                        track_select[chan]->set(OFF);
                    break;
                    case 52:
                        clip_stop[chan]->set(OFF);
                    break;
                    case 53:
                    case 54:
                    case 55:
                    case 56:
                    case 57:
                        byte2 -= 53;
                        clip_launch[chan][byte2]->set(OFF);
                    break;
                case 81:
                    shift->set(OFF);
                break;
                case 82:
                case 83:
                case 84:
                case 85:
                case 86:
                scene_launch[byte2-82]->set(OFF);
                break;
                }
            break;
            case CC_ABSOLUTE:
                vsx_printf("CC_ABSOLUTE: %X VALUE: %X\n", byte2, byte3);
                fflush(stdout);
                float val = (float)(byte3/127.0f);
                if(byte2==CC_ID_TRACK){
                    fader[chan]->set(val);
                } else if (byte2==CC_ID_MASTER){
                    fader[8]->set(val);
                } else if (byte2==CC_ID_KNOB){
                    knob->set((float)byte3);
                }
            break;
            }

            handle_messages();
        }
    }
private:
     vsx_string remove_spaces(std::string st)
    {
         vsx_string str = st.c_str();
        for(size_t i=0;i<st.length();i++)
        {
            if(str[i]==' '||str[i]==':')
            {
                str[i]='_';
            }
        }
        return str;
    }
     int get_port()
     {
         int i = midi_source->get();
         if(i > port_count){
             return port_count;
         } else {
             return i;
         }
     }

};

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************

int rtmidi_type = 0;



//******************************************************************************
//*** F A C T O R Y ************************************************************
//******************************************************************************

#ifndef _WIN32
#define __declspec(a)
#endif

extern "C"
{
  __declspec(dllexport) void print_help();
  __declspec(dllexport) vsx_module* create_new_module(unsigned long module, void* args);
  __declspec(dllexport) void destroy_module(vsx_module* m,unsigned long module);
  __declspec(dllexport) unsigned long get_num_modules();
  __declspec(dllexport) void on_unload_library();
}

void print_help()
{
  printf("Parameters for vsx_listener (visualization sound input):\n");
  #if (PLATFORM == PLATFORM_LINUX)
  printf("  PulseAudio is the default sound driver, but you can override with any\n"
         "  of the following:\n\n");
  #if defined(__LINUX_ALSA__)
  printf("  -sound_type_alsa            - ALSA sound system\n");
  #endif

  #if defined(__LINUX_JACK__)
  printf("  -sound_type_jack            - JACK sound system\n");
  #endif

  #if defined(__LINUX_OSS__)
  printf("  -sound_type_oss             - OSS sound system\n");
  #endif
  #endif
}


vsx_module* create_new_module(unsigned long module, void* args)
{
  vsx_argvector* internal_args = (vsx_argvector*) args;
  switch(module)
  {
    case 0:
      if (internal_args->has_param("sound_type_jack"))
      {
        // JACK
        rtmidi_type = RtMidi::UNIX_JACK;
      }  else
      {
        // default - alsa
        rtmidi_type = RtMidi::LINUX_ALSA;
      }
      return (vsx_module*)(new vsx_apc_fourty);
    }
  print_help();
  return 0;
}

void destroy_module(vsx_module* m,unsigned long module)
{
  switch(module)
  {
    case 0:
    return delete (vsx_apc_fourty*)m;
  }
}

unsigned long get_num_modules() {
  return 1;
}

