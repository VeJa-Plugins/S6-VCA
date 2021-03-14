#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <lv2.h>

#include "lv2/lv2plug.in/ns/ext/atom/atom.h"
#include "lv2/lv2plug.in/ns/ext/atom/util.h"
#include "lv2/lv2plug.in/ns/ext/midi/midi.h"
#include "lv2/lv2plug.in/ns/ext/urid/urid.h"
#include "lv2/lv2plug.in/ns/lv2core/lv2.h"

/**********************************************************************************************************************************************************/
#define PLUGIN_URI "http://VeJaPlugins.com/plugins/Release/s6Vca"
#define PI 3.14159265358979323846264338327950288
#define MAX_PORTS 8
#define MAX_OUTPUT_BUFFER_LENGHT 256
#define VCF_LOW_PASS_MODE 0


enum{
    INPUT,
    CvEnvInput,
    CvLFOInput,
    OUTPUT,
    LFO_MOD,
    LEVEL
};


class S6_vca{
public:
    S6_vca()
    {

    }
    ~S6_vca() {}
    static LV2_Handle instantiate(const LV2_Descriptor* descriptor, double samplerate, const char* bundle_path, const LV2_Feature* const* features);
    static void activate(LV2_Handle instance);
    static void deactivate(LV2_Handle instance);
    static void connect_port(LV2_Handle instance, uint32_t port, void *data);
    static void run(LV2_Handle instance, uint32_t n_samples);
    static void cleanup(LV2_Handle instance);
    static const void* extension_data(const char* uri);
    
    // Features
    LV2_URID_Map* map;
    LV2_URID urid_midiEvent;    
        
    //audio ports
    const LV2_Atom_Sequence* port_events_in;
    float *input;
    float *output;
    float *lfo_signal;
    float *env_signal;
    float *lfo_mod;
    float *lvl;
};
/**********************************************************************************************************************************************************/


/**********************************************************************************************************************************************************/
LV2_Handle S6_vca::instantiate(const LV2_Descriptor*   descriptor,
double                              samplerate,
const char*                         bundle_path,
const LV2_Feature* const* features)
{
    S6_vca* self = new S6_vca();

    return (LV2_Handle)self; 
}
/**********************************************************************************************************************************************************/
void S6_vca::connect_port(LV2_Handle instance, uint32_t port, void *data)
{
    S6_vca* self = (S6_vca*)instance;
    switch (port)
    {
        case INPUT:
            self->input = (float*) data;
            break;
        case CvEnvInput:
            self->lfo_signal = (float*) data;
            break;
        case CvLFOInput:
            self->env_signal = (float*) data;
            break;
        case OUTPUT:
            self->output = (float*) data;
            break;
        case LFO_MOD:
            self->lfo_mod = (float*) data;
            break;
        case LEVEL:
            self->lvl = (float*) data;
            break;

    }
}
/**********************************************************************************************************************************************************/
void S6_vca::activate(LV2_Handle instance)
{
}

/**********************************************************************************************************************************************************/
void S6_vca::run(LV2_Handle instance, uint32_t n_samples)
{
    S6_vca* self = (S6_vca*)instance;

  	//start audio processing
    for(uint32_t i = 0; i < n_samples; i++)
    {
        self->output[i] = self->input[i] * ((((float)*self->lvl / 10) * ((float)self->env_signal[i]/10)) + (((float)*self->lfo_mod / 10) * ((float)self->lfo_signal[i] / 10)));
    }
}   

/**********************************************************************************************************************************************************/
void S6_vca::deactivate(LV2_Handle instance)
{
    // TODO: include the deactivate function code here
}
/**********************************************************************************************************************************************************/
void S6_vca::cleanup(LV2_Handle instance)
{
  delete ((S6_vca *) instance); 
}
/**********************************************************************************************************************************************************/
static const LV2_Descriptor Descriptor = {
    PLUGIN_URI,
    S6_vca::instantiate,
    S6_vca::connect_port,
    S6_vca::activate,
    S6_vca::run,
    S6_vca::deactivate,
    S6_vca::cleanup,
    S6_vca::extension_data
};
/**********************************************************************************************************************************************************/
LV2_SYMBOL_EXPORT
const LV2_Descriptor* lv2_descriptor(uint32_t index)
{
    if (index == 0) return &Descriptor;
    else return NULL;
}
/**********************************************************************************************************************************************************/
const void* S6_vca::extension_data(const char* uri)
{
    return NULL;
}
