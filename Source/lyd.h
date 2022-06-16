/* ------------------------------------------------------------
name: "LydNY22"
Code generated with Faust 2.30.8 (https://faust.grame.fr)
Compilation options: -lang cpp -es 1 -scal -ftz 0
------------------------------------------------------------ */

#ifndef  __mydsp_H__
#define  __mydsp_H__

#include <cmath>
#include <cstring>

/************************** BEGIN dsp.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __dsp__
#define __dsp__

#include <string>
#include <vector>

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

struct UI;
struct Meta;

/**
 * DSP memory manager.
 */

struct dsp_memory_manager {
    
    virtual ~dsp_memory_manager() {}
    
    virtual void* allocate(size_t size) = 0;
    virtual void destroy(void* ptr) = 0;
    
};

/**
* Signal processor definition.
*/

class dsp {

    public:

        dsp() {}
        virtual ~dsp() {}

        /* Return instance number of audio inputs */
        virtual int getNumInputs() = 0;
    
        /* Return instance number of audio outputs */
        virtual int getNumOutputs() = 0;
    
        /**
         * Trigger the ui_interface parameter with instance specific calls
         * to 'openTabBox', 'addButton', 'addVerticalSlider'... in order to build the UI.
         *
         * @param ui_interface - the user interface builder
         */
        virtual void buildUserInterface(UI* ui_interface) = 0;
    
        /* Return the sample rate currently used by the instance */
        virtual int getSampleRate() = 0;
    
        /**
         * Global init, calls the following methods:
         * - static class 'classInit': static tables initialization
         * - 'instanceInit': constants and instance state initialization
         *
         * @param sample_rate - the sampling rate in Hz
         */
        virtual void init(int sample_rate) = 0;

        /**
         * Init instance state
         *
         * @param sample_rate - the sampling rate in Hz
         */
        virtual void instanceInit(int sample_rate) = 0;
    
        /**
         * Init instance constant state
         *
         * @param sample_rate - the sampling rate in Hz
         */
        virtual void instanceConstants(int sample_rate) = 0;
    
        /* Init default control parameters values */
        virtual void instanceResetUserInterface() = 0;
    
        /* Init instance state (like delay lines...) but keep the control parameter values */
        virtual void instanceClear() = 0;
 
        /**
         * Return a clone of the instance.
         *
         * @return a copy of the instance on success, otherwise a null pointer.
         */
        virtual dsp* clone() = 0;
    
        /**
         * Trigger the Meta* parameter with instance specific calls to 'declare' (key, value) metadata.
         *
         * @param m - the Meta* meta user
         */
        virtual void metadata(Meta* m) = 0;
    
        /**
         * DSP instance computation, to be called with successive in/out audio buffers.
         *
         * @param count - the number of frames to compute
         * @param inputs - the input audio buffers as an array of non-interleaved FAUSTFLOAT samples (eiher float, double or quad)
         * @param outputs - the output audio buffers as an array of non-interleaved FAUSTFLOAT samples (eiher float, double or quad)
         *
         */
        virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) = 0;
    
        /**
         * DSP instance computation: alternative method to be used by subclasses.
         *
         * @param date_usec - the timestamp in microsec given by audio driver.
         * @param count - the number of frames to compute
         * @param inputs - the input audio buffers as an array of non-interleaved FAUSTFLOAT samples (either float, double or quad)
         * @param outputs - the output audio buffers as an array of non-interleaved FAUSTFLOAT samples (either float, double or quad)
         *
         */
        virtual void compute(double /*date_usec*/, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) { compute(count, inputs, outputs); }
       
};

/**
 * Generic DSP decorator.
 */

class decorator_dsp : public dsp {

    protected:

        dsp* fDSP;

    public:

        decorator_dsp(dsp* dsp = nullptr):fDSP(dsp) {}
        virtual ~decorator_dsp() { delete fDSP; }

        virtual int getNumInputs() { return fDSP->getNumInputs(); }
        virtual int getNumOutputs() { return fDSP->getNumOutputs(); }
        virtual void buildUserInterface(UI* ui_interface) { fDSP->buildUserInterface(ui_interface); }
        virtual int getSampleRate() { return fDSP->getSampleRate(); }
        virtual void init(int sample_rate) { fDSP->init(sample_rate); }
        virtual void instanceInit(int sample_rate) { fDSP->instanceInit(sample_rate); }
        virtual void instanceConstants(int sample_rate) { fDSP->instanceConstants(sample_rate); }
        virtual void instanceResetUserInterface() { fDSP->instanceResetUserInterface(); }
        virtual void instanceClear() { fDSP->instanceClear(); }
        virtual decorator_dsp* clone() { return new decorator_dsp(fDSP->clone()); }
        virtual void metadata(Meta* m) { fDSP->metadata(m); }
        // Beware: subclasses usually have to overload the two 'compute' methods
        virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) { fDSP->compute(count, inputs, outputs); }
        virtual void compute(double date_usec, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) { fDSP->compute(date_usec, count, inputs, outputs); }
    
};

/**
 * DSP factory class, used with LLVM and Interpreter backends
 * to create DSP instances from a compiled DSP program.
 */

class dsp_factory {
    
    protected:
    
        // So that to force sub-classes to use deleteDSPFactory(dsp_factory* factory);
        virtual ~dsp_factory() {}
    
    public:
    
        virtual std::string getName() = 0;
        virtual std::string getSHAKey() = 0;
        virtual std::string getDSPCode() = 0;
        virtual std::string getCompileOptions() = 0;
        virtual std::vector<std::string> getLibraryList() = 0;
        virtual std::vector<std::string> getIncludePathnames() = 0;
    
        virtual dsp* createDSPInstance() = 0;
    
        virtual void setMemoryManager(dsp_memory_manager* manager) = 0;
        virtual dsp_memory_manager* getMemoryManager() = 0;
    
};

/**
 * On Intel set FZ (Flush to Zero) and DAZ (Denormals Are Zero)
 * flags to avoid costly denormals.
 */

#ifdef __SSE__
    #include <xmmintrin.h>
    #ifdef __SSE2__
        #define AVOIDDENORMALS _mm_setcsr(_mm_getcsr() | 0x8040)
    #else
        #define AVOIDDENORMALS _mm_setcsr(_mm_getcsr() | 0x8000)
    #endif
#else
    #define AVOIDDENORMALS
#endif

#endif
/**************************  END  dsp.h **************************/
/************************** BEGIN MapUI.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef FAUST_MAPUI_H
#define FAUST_MAPUI_H

#include <vector>
#include <map>
#include <string>

/************************** BEGIN UI.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2020 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __UI_H__
#define __UI_H__

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

/*******************************************************************************
 * UI : Faust DSP User Interface
 * User Interface as expected by the buildUserInterface() method of a DSP.
 * This abstract class contains only the method that the Faust compiler can
 * generate to describe a DSP user interface.
 ******************************************************************************/

struct Soundfile;

template <typename REAL>
struct UIReal
{
    UIReal() {}
    virtual ~UIReal() {}
    
    // -- widget's layouts
    
    virtual void openTabBox(const char* label) = 0;
    virtual void openHorizontalBox(const char* label) = 0;
    virtual void openVerticalBox(const char* label) = 0;
    virtual void closeBox() = 0;
    
    // -- active widgets
    
    virtual void addButton(const char* label, REAL* zone) = 0;
    virtual void addCheckButton(const char* label, REAL* zone) = 0;
    virtual void addVerticalSlider(const char* label, REAL* zone, REAL init, REAL min, REAL max, REAL step) = 0;
    virtual void addHorizontalSlider(const char* label, REAL* zone, REAL init, REAL min, REAL max, REAL step) = 0;
    virtual void addNumEntry(const char* label, REAL* zone, REAL init, REAL min, REAL max, REAL step) = 0;
    
    // -- passive widgets
    
    virtual void addHorizontalBargraph(const char* label, REAL* zone, REAL min, REAL max) = 0;
    virtual void addVerticalBargraph(const char* label, REAL* zone, REAL min, REAL max) = 0;
    
    // -- soundfiles
    
    virtual void addSoundfile(const char* label, const char* filename, Soundfile** sf_zone) = 0;
    
    // -- metadata declarations
    
    virtual void declare(REAL* zone, const char* key, const char* val) {}
};

struct UI : public UIReal<FAUSTFLOAT>
{
    UI() {}
    virtual ~UI() {}
};

#endif
/**************************  END  UI.h **************************/
/************************** BEGIN PathBuilder.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef FAUST_PATHBUILDER_H
#define FAUST_PATHBUILDER_H

#include <vector>
#include <string>
#include <algorithm>

/*******************************************************************************
 * PathBuilder : Faust User Interface
 * Helper class to build complete hierarchical path for UI items.
 ******************************************************************************/

class PathBuilder
{

    protected:
    
        std::vector<std::string> fControlsLevel;
       
    public:
    
        PathBuilder() {}
        virtual ~PathBuilder() {}
    
        std::string buildPath(const std::string& label) 
        {
            std::string res = "/";
            for (size_t i = 0; i < fControlsLevel.size(); i++) {
                res += fControlsLevel[i];
                res += "/";
            }
            res += label;
            std::replace(res.begin(), res.end(), ' ', '_');
            return res;
        }
    
        std::string buildLabel(std::string label)
        {
            std::replace(label.begin(), label.end(), ' ', '_');
            return label;
        }
    
        void pushLabel(const std::string& label) { fControlsLevel.push_back(label); }
        void popLabel() { fControlsLevel.pop_back(); }
    
};

#endif  // FAUST_PATHBUILDER_H
/**************************  END  PathBuilder.h **************************/

/*******************************************************************************
 * MapUI : Faust User Interface
 * This class creates a map of complete hierarchical path and zones for each UI items.
 ******************************************************************************/

class MapUI : public UI, public PathBuilder
{
    
    protected:
    
        // Complete path map
        std::map<std::string, FAUSTFLOAT*> fPathZoneMap;
    
        // Label zone map
        std::map<std::string, FAUSTFLOAT*> fLabelZoneMap;
    
        std::string fNullStr = "";
    
    public:
        
        MapUI() {}
        virtual ~MapUI() {}
        
        // -- widget's layouts
        void openTabBox(const char* label)
        {
            pushLabel(label);
        }
        void openHorizontalBox(const char* label)
        {
            pushLabel(label);
        }
        void openVerticalBox(const char* label)
        {
            pushLabel(label);
        }
        void closeBox()
        {
            popLabel();
        }
        
        // -- active widgets
        void addButton(const char* label, FAUSTFLOAT* zone)
        {
            fPathZoneMap[buildPath(label)] = zone;
            fLabelZoneMap[label] = zone;
        }
        void addCheckButton(const char* label, FAUSTFLOAT* zone)
        {
            fPathZoneMap[buildPath(label)] = zone;
            fLabelZoneMap[label] = zone;
        }
        void addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT fmin, FAUSTFLOAT fmax, FAUSTFLOAT step)
        {
            fPathZoneMap[buildPath(label)] = zone;
            fLabelZoneMap[label] = zone;
        }
        void addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT fmin, FAUSTFLOAT fmax, FAUSTFLOAT step)
        {
            fPathZoneMap[buildPath(label)] = zone;
            fLabelZoneMap[label] = zone;
        }
        void addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT fmin, FAUSTFLOAT fmax, FAUSTFLOAT step)
        {
            fPathZoneMap[buildPath(label)] = zone;
            fLabelZoneMap[label] = zone;
        }
        
        // -- passive widgets
        void addHorizontalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT fmin, FAUSTFLOAT fmax)
        {
            fPathZoneMap[buildPath(label)] = zone;
            fLabelZoneMap[label] = zone;
        }
        void addVerticalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT fmin, FAUSTFLOAT fmax)
        {
            fPathZoneMap[buildPath(label)] = zone;
            fLabelZoneMap[label] = zone;
        }
    
        // -- soundfiles
        virtual void addSoundfile(const char* label, const char* filename, Soundfile** sf_zone) {}
        
        // -- metadata declarations
        virtual void declare(FAUSTFLOAT* zone, const char* key, const char* val)
        {}
        
        // set/get
        void setParamValue(const std::string& path, FAUSTFLOAT value)
        {
            if (fPathZoneMap.find(path) != fPathZoneMap.end()) {
                *fPathZoneMap[path] = value;
            } else if (fLabelZoneMap.find(path) != fLabelZoneMap.end()) {
                *fLabelZoneMap[path] = value;
            }
        }
        
        FAUSTFLOAT getParamValue(const std::string& path)
        {
            if (fPathZoneMap.find(path) != fPathZoneMap.end()) {
                return *fPathZoneMap[path];
            } else if (fLabelZoneMap.find(path) != fLabelZoneMap.end()) {
                return *fLabelZoneMap[path];
            } else {
                return FAUSTFLOAT(0);
            }
        }
    
        // map access 
        std::map<std::string, FAUSTFLOAT*>& getMap() { return fPathZoneMap; }
        
        int getParamsCount() { return int(fPathZoneMap.size()); }
        
        const std::string& getParamAddress(int index)
        {
            if (index < 0 || index > int(fPathZoneMap.size())) {
                return fNullStr;
            } else {
                auto it = fPathZoneMap.begin();
                while (index-- > 0 && it++ != fPathZoneMap.end()) {}
                return it->first;
            }
        }
    
        const std::string& getParamAddress(FAUSTFLOAT* zone)
        {
            for (auto& it : fPathZoneMap) {
                if (it.second == zone) return it.first;
            }
            return fNullStr;
        }
    
        FAUSTFLOAT* getParamZone(const std::string& str)
        {
            if (fPathZoneMap.find(str) != fPathZoneMap.end()) {
                return fPathZoneMap[str];
            }
            if (fLabelZoneMap.find(str) != fLabelZoneMap.end()) {
                return fLabelZoneMap[str];
            }
            return nullptr;
        }
    
        FAUSTFLOAT* getParamZone(int index)
        {
            if (index < 0 || index > int(fPathZoneMap.size())) {
                return nullptr;
            } else {
                auto it = fPathZoneMap.begin();
                while (index-- > 0 && it++ != fPathZoneMap.end()) {}
                return it->second;
            }
        }
    
        static bool endsWith(const std::string& str, const std::string& end)
        {
            size_t l1 = str.length();
            size_t l2 = end.length();
            return (l1 >= l2) && (0 == str.compare(l1 - l2, l2, end));
        }
};


#endif // FAUST_MAPUI_H
/**************************  END  MapUI.h **************************/
/************************** BEGIN meta.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __meta__
#define __meta__

/**
 The base class of Meta handler to be used in dsp::metadata(Meta* m) method to retrieve (key, value) metadata.
 */
struct Meta
{
    virtual ~Meta() {};
    virtual void declare(const char* key, const char* value) = 0;
};

#endif
/**************************  END  meta.h **************************/

// BEGIN-FAUSTDSP

    // END-FAUSTDSP
#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif 

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <math.h>

class mydspSIG0 {
	
  private:
	
	int iRec3[2];
	
  public:
	
	int getNumInputsmydspSIG0() {
		return 0;
	}
	int getNumOutputsmydspSIG0() {
		return 1;
	}
	int getInputRatemydspSIG0(int channel) {
		int rate;
		switch ((channel)) {
			default: {
				rate = -1;
				break;
			}
		}
		return rate;
	}
	int getOutputRatemydspSIG0(int channel) {
		int rate;
		switch ((channel)) {
			case 0: {
				rate = 0;
				break;
			}
			default: {
				rate = -1;
				break;
			}
		}
		return rate;
	}
	
	void instanceInitmydspSIG0(int sample_rate) {
		for (int l4 = 0; (l4 < 2); l4 = (l4 + 1)) {
			iRec3[l4] = 0;
		}
	}
	
	void fillmydspSIG0(int count, float* table) {
		for (int i = 0; (i < count); i = (i + 1)) {
			iRec3[0] = (iRec3[1] + 1);
			table[i] = std::sin((9.58738019e-05f * float((iRec3[0] + -1))));
			iRec3[1] = iRec3[0];
		}
	}

};

static mydspSIG0* newmydspSIG0() { return (mydspSIG0*)new mydspSIG0(); }
static void deletemydspSIG0(mydspSIG0* dsp) { delete dsp; }

static float ftbl0mydspSIG0[65536];
static float mydsp_faustpower2_f(float value) {
	return (value * value);
}

#ifndef FAUSTCLASS 
#define FAUSTCLASS mydsp
#endif

#ifdef __APPLE__ 
#define exp10f __exp10f
#define exp10 __exp10
#endif

class mydsp : public dsp {
	
 private:
	
	FAUSTFLOAT fVslider0;
	float fRec0[2];
	FAUSTFLOAT fVslider1;
	float fRec1[2];
	int fSampleRate;
	float fConst0;
	float fConst1;
	float fConst2;
	FAUSTFLOAT fButton0;
	float fVec0[2];
	int iRec2[2];
	FAUSTFLOAT fHslider0;
	float fConst3;
	float fRec4[2];
	float fConst4;
	float fRec5[2];
	float fConst5;
	float fRec6[2];
	float fConst6;
	float fRec7[2];
	float fConst7;
	float fRec8[2];
	float fConst8;
	float fRec9[2];
	float fConst9;
	float fRec10[2];
	float fConst10;
	float fRec11[2];
	float fConst11;
	float fRec12[2];
	float fConst12;
	float fRec13[2];
	float fConst13;
	float fRec14[2];
	float fConst14;
	float fRec15[2];
	float fConst15;
	float fRec16[2];
	float fConst16;
	float fRec17[2];
	float fConst17;
	float fRec18[2];
	FAUSTFLOAT fButton1;
	float fVec1[2];
	int iRec19[2];
	float fConst18;
	float fRec20[2];
	float fConst19;
	float fRec21[2];
	float fConst20;
	float fRec22[2];
	float fConst21;
	float fRec23[2];
	float fConst22;
	float fRec24[2];
	float fConst23;
	float fRec25[2];
	float fConst24;
	float fRec26[2];
	float fConst25;
	float fRec27[2];
	float fConst26;
	float fRec28[2];
	float fConst27;
	float fRec29[2];
	float fConst28;
	float fRec30[2];
	float fConst29;
	float fRec31[2];
	float fConst30;
	float fRec32[2];
	float fConst31;
	float fRec33[2];
	float fConst32;
	float fRec34[2];
	FAUSTFLOAT fButton2;
	float fVec2[2];
	int iRec35[2];
	float fConst33;
	float fRec36[2];
	float fConst34;
	float fRec37[2];
	float fConst35;
	float fRec38[2];
	float fConst36;
	float fRec39[2];
	float fConst37;
	float fRec40[2];
	float fConst38;
	float fRec41[2];
	float fConst39;
	float fRec42[2];
	float fConst40;
	float fRec43[2];
	float fConst41;
	float fRec44[2];
	float fConst42;
	float fRec45[2];
	float fConst43;
	float fRec46[2];
	float fConst44;
	float fRec47[2];
	float fConst45;
	float fRec48[2];
	float fConst46;
	float fRec49[2];
	float fConst47;
	float fRec50[2];
	FAUSTFLOAT fButton3;
	float fVec3[2];
	int iRec51[2];
	float fConst48;
	float fRec52[2];
	float fConst49;
	float fRec53[2];
	float fConst50;
	float fRec54[2];
	float fConst51;
	float fRec55[2];
	float fConst52;
	float fRec56[2];
	float fConst53;
	float fRec57[2];
	float fConst54;
	float fRec58[2];
	float fConst55;
	float fRec59[2];
	float fConst56;
	float fRec60[2];
	float fConst57;
	float fRec61[2];
	float fConst58;
	float fRec62[2];
	float fConst59;
	float fRec63[2];
	float fConst60;
	float fRec64[2];
	float fConst61;
	float fRec65[2];
	float fConst62;
	float fRec66[2];
	FAUSTFLOAT fButton4;
	float fVec4[2];
	int iRec67[2];
	float fConst63;
	float fRec68[2];
	float fConst64;
	float fRec69[2];
	float fConst65;
	float fRec70[2];
	float fConst66;
	float fRec71[2];
	float fConst67;
	float fRec72[2];
	float fConst68;
	float fRec73[2];
	float fConst69;
	float fRec74[2];
	float fConst70;
	float fRec75[2];
	float fConst71;
	float fRec76[2];
	float fConst72;
	float fRec77[2];
	float fConst73;
	float fRec78[2];
	float fConst74;
	float fRec79[2];
	float fConst75;
	float fRec80[2];
	float fConst76;
	float fRec81[2];
	float fConst77;
	float fRec82[2];
	FAUSTFLOAT fButton5;
	float fVec5[2];
	int iRec83[2];
	float fConst78;
	float fRec84[2];
	float fConst79;
	float fRec85[2];
	float fConst80;
	float fRec86[2];
	float fConst81;
	float fRec87[2];
	float fConst82;
	float fRec88[2];
	float fConst83;
	float fRec89[2];
	float fConst84;
	float fRec90[2];
	float fConst85;
	float fRec91[2];
	float fConst86;
	float fRec92[2];
	float fConst87;
	float fRec93[2];
	float fConst88;
	float fRec94[2];
	float fConst89;
	float fRec95[2];
	float fConst90;
	float fRec96[2];
	float fConst91;
	float fRec97[2];
	float fConst92;
	float fRec98[2];
	FAUSTFLOAT fButton6;
	float fVec6[2];
	int iRec99[2];
	float fConst93;
	float fRec100[2];
	float fConst94;
	float fRec101[2];
	float fConst95;
	float fRec102[2];
	float fConst96;
	float fRec103[2];
	float fConst97;
	float fRec104[2];
	float fConst98;
	float fRec105[2];
	float fConst99;
	float fRec106[2];
	float fConst100;
	float fRec107[2];
	float fConst101;
	float fRec108[2];
	float fConst102;
	float fRec109[2];
	float fConst103;
	float fRec110[2];
	float fConst104;
	float fRec111[2];
	float fConst105;
	float fRec112[2];
	float fConst106;
	float fRec113[2];
	float fConst107;
	float fRec114[2];
	float fConst109;
	float fConst111;
	float fConst121;
	float fConst122;
	float fConst123;
	float fConst125;
	float fConst126;
	float fRec128[2];
	float fRec127[2];
	int IOTA;
	float fVec7[32768];
	int iConst128;
	float fVec8[16384];
	int iConst129;
	float fVec9[2048];
	int iConst130;
	float fRec125[2];
	float fConst139;
	float fConst140;
	float fConst141;
	float fRec132[2];
	float fRec131[2];
	float fVec10[32768];
	int iConst143;
	float fVec11[4096];
	int iConst144;
	float fRec129[2];
	float fConst153;
	float fConst154;
	float fConst155;
	float fRec136[2];
	float fRec135[2];
	float fVec12[16384];
	int iConst157;
	float fVec13[4096];
	int iConst158;
	float fRec133[2];
	float fConst167;
	float fConst168;
	float fConst169;
	float fRec140[2];
	float fRec139[2];
	float fVec14[32768];
	int iConst171;
	float fVec15[4096];
	int iConst172;
	float fRec137[2];
	float fConst181;
	float fConst182;
	float fConst183;
	float fRec144[2];
	float fRec143[2];
	float fVec16[16384];
	int iConst185;
	float fVec17[2048];
	int iConst186;
	float fRec141[2];
	float fConst195;
	float fConst196;
	float fConst197;
	float fRec148[2];
	float fRec147[2];
	float fVec18[16384];
	int iConst199;
	float fVec19[4096];
	int iConst200;
	float fRec145[2];
	float fConst209;
	float fConst210;
	float fConst211;
	float fRec152[2];
	float fRec151[2];
	float fVec20[16384];
	int iConst213;
	float fVec21[4096];
	int iConst214;
	float fRec149[2];
	float fConst223;
	float fConst224;
	float fConst225;
	float fRec156[2];
	float fRec155[2];
	float fVec22[16384];
	int iConst227;
	float fVec23[2048];
	int iConst228;
	float fRec153[2];
	float fRec117[3];
	float fRec118[3];
	float fRec119[3];
	float fRec120[3];
	float fRec121[3];
	float fRec122[3];
	float fRec123[3];
	float fRec124[3];
	float fConst229;
	float fRec116[3];
	float fConst230;
	float fRec115[3];
	float fRec158[3];
	float fRec157[3];
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "0.1");
		m->declare("compile_options", "-lang cpp -es 1 -scal -ftz 0");
		m->declare("delays.lib/name", "Faust Delay Library");
		m->declare("delays.lib/version", "0.1");
		m->declare("envelopes.lib/ar:author", "Yann Orlarey, Stéphane Letz");
		m->declare("envelopes.lib/author", "GRAME");
		m->declare("envelopes.lib/copyright", "GRAME");
		m->declare("envelopes.lib/license", "LGPL with exception");
		m->declare("envelopes.lib/name", "Faust Envelope Library");
		m->declare("envelopes.lib/version", "0.1");
		m->declare("filename", "LydNY22.dsp");
		m->declare("filters.lib/allpass_comb:author", "Julius O. Smith III");
		m->declare("filters.lib/allpass_comb:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/allpass_comb:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/fir:author", "Julius O. Smith III");
		m->declare("filters.lib/fir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/fir:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/iir:author", "Julius O. Smith III");
		m->declare("filters.lib/iir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/iir:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/lowpass0_highpass1", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/lowpass0_highpass1:author", "Julius O. Smith III");
		m->declare("filters.lib/lowpass:author", "Julius O. Smith III");
		m->declare("filters.lib/lowpass:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/lowpass:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/name", "Faust Filters Library");
		m->declare("filters.lib/peak_eq_rm:author", "Julius O. Smith III");
		m->declare("filters.lib/peak_eq_rm:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/peak_eq_rm:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf1:author", "Julius O. Smith III");
		m->declare("filters.lib/tf1:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf1:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf1s:author", "Julius O. Smith III");
		m->declare("filters.lib/tf1s:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf1s:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf2:author", "Julius O. Smith III");
		m->declare("filters.lib/tf2:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf2:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/version", "0.3");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.3");
		m->declare("misceffects.lib/name", "Misc Effects Library");
		m->declare("misceffects.lib/version", "2.0");
		m->declare("name", "LydNY22");
		m->declare("oscillators.lib/name", "Faust Oscillator Library");
		m->declare("oscillators.lib/version", "0.1");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "0.1");
		m->declare("reverbs.lib/name", "Faust Reverb Library");
		m->declare("reverbs.lib/version", "0.0");
		m->declare("routes.lib/name", "Faust Signal Routing Library");
		m->declare("routes.lib/version", "0.2");
		m->declare("signals.lib/name", "Faust Signal Routing Library");
		m->declare("signals.lib/version", "0.0");
	}

	virtual int getNumInputs() {
		return 0;
	}
	virtual int getNumOutputs() {
		return 2;
	}
	virtual int getInputRate(int channel) {
		int rate;
		switch ((channel)) {
			default: {
				rate = -1;
				break;
			}
		}
		return rate;
	}
	virtual int getOutputRate(int channel) {
		int rate;
		switch ((channel)) {
			case 0: {
				rate = 1;
				break;
			}
			case 1: {
				rate = 1;
				break;
			}
			default: {
				rate = -1;
				break;
			}
		}
		return rate;
	}
	
	static void classInit(int sample_rate) {
		mydspSIG0* sig0 = newmydspSIG0();
		sig0->instanceInitmydspSIG0(sample_rate);
		sig0->fillmydspSIG0(65536, ftbl0mydspSIG0);
		deletemydspSIG0(sig0);
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		fConst0 = std::min<float>(192000.0f, std::max<float>(1.0f, float(fSampleRate)));
		fConst1 = std::max<float>(1.0f, (0.00999999978f * fConst0));
		fConst2 = (1.0f / fConst1);
		fConst3 = (220.0f / fConst0);
		fConst4 = (440.0f / fConst0);
		fConst5 = (660.0f / fConst0);
		fConst6 = (880.0f / fConst0);
		fConst7 = (1100.0f / fConst0);
		fConst8 = (1320.0f / fConst0);
		fConst9 = (1540.0f / fConst0);
		fConst10 = (1760.0f / fConst0);
		fConst11 = (1980.0f / fConst0);
		fConst12 = (2200.0f / fConst0);
		fConst13 = (2420.0f / fConst0);
		fConst14 = (2640.0f / fConst0);
		fConst15 = (2860.0f / fConst0);
		fConst16 = (3080.0f / fConst0);
		fConst17 = (3300.0f / fConst0);
		fConst18 = (246.940002f / fConst0);
		fConst19 = (493.880005f / fConst0);
		fConst20 = (740.820007f / fConst0);
		fConst21 = (987.76001f / fConst0);
		fConst22 = (1234.69995f / fConst0);
		fConst23 = (1481.64001f / fConst0);
		fConst24 = (1728.57996f / fConst0);
		fConst25 = (1975.52002f / fConst0);
		fConst26 = (2222.45996f / fConst0);
		fConst27 = (2469.3999f / fConst0);
		fConst28 = (2716.34009f / fConst0);
		fConst29 = (2963.28003f / fConst0);
		fConst30 = (3210.21997f / fConst0);
		fConst31 = (3457.15991f / fConst0);
		fConst32 = (3704.1001f / fConst0);
		fConst33 = (261.630005f / fConst0);
		fConst34 = (523.26001f / fConst0);
		fConst35 = (784.890015f / fConst0);
		fConst36 = (1046.52002f / fConst0);
		fConst37 = (1308.15002f / fConst0);
		fConst38 = (1569.78003f / fConst0);
		fConst39 = (1831.41003f / fConst0);
		fConst40 = (2093.04004f / fConst0);
		fConst41 = (2354.66992f / fConst0);
		fConst42 = (2616.30005f / fConst0);
		fConst43 = (2877.92993f / fConst0);
		fConst44 = (3139.56006f / fConst0);
		fConst45 = (3401.18994f / fConst0);
		fConst46 = (3662.82007f / fConst0);
		fConst47 = (3924.44995f / fConst0);
		fConst48 = (293.660004f / fConst0);
		fConst49 = (587.320007f / fConst0);
		fConst50 = (880.97998f / fConst0);
		fConst51 = (1174.64001f / fConst0);
		fConst52 = (1468.30005f / fConst0);
		fConst53 = (1761.95996f / fConst0);
		fConst54 = (2055.62012f / fConst0);
		fConst55 = (2349.28003f / fConst0);
		fConst56 = (2642.93994f / fConst0);
		fConst57 = (2936.6001f / fConst0);
		fConst58 = (3230.26001f / fConst0);
		fConst59 = (3523.91992f / fConst0);
		fConst60 = (3817.58008f / fConst0);
		fConst61 = (4111.24023f / fConst0);
		fConst62 = (4404.8999f / fConst0);
		fConst63 = (329.630005f / fConst0);
		fConst64 = (659.26001f / fConst0);
		fConst65 = (988.890015f / fConst0);
		fConst66 = (1318.52002f / fConst0);
		fConst67 = (1648.15002f / fConst0);
		fConst68 = (1977.78003f / fConst0);
		fConst69 = (2307.40991f / fConst0);
		fConst70 = (2637.04004f / fConst0);
		fConst71 = (2966.66992f / fConst0);
		fConst72 = (3296.30005f / fConst0);
		fConst73 = (3625.92993f / fConst0);
		fConst74 = (3955.56006f / fConst0);
		fConst75 = (4285.18994f / fConst0);
		fConst76 = (4614.81982f / fConst0);
		fConst77 = (4944.4502f / fConst0);
		fConst78 = (349.230011f / fConst0);
		fConst79 = (698.460022f / fConst0);
		fConst80 = (1047.68994f / fConst0);
		fConst81 = (1396.92004f / fConst0);
		fConst82 = (1746.15002f / fConst0);
		fConst83 = (2095.37988f / fConst0);
		fConst84 = (2444.61011f / fConst0);
		fConst85 = (2793.84009f / fConst0);
		fConst86 = (3143.07007f / fConst0);
		fConst87 = (3492.30005f / fConst0);
		fConst88 = (3841.53003f / fConst0);
		fConst89 = (4190.75977f / fConst0);
		fConst90 = (4539.99023f / fConst0);
		fConst91 = (4889.22021f / fConst0);
		fConst92 = (5238.4502f / fConst0);
		fConst93 = (392.0f / fConst0);
		fConst94 = (784.0f / fConst0);
		fConst95 = (1176.0f / fConst0);
		fConst96 = (1568.0f / fConst0);
		fConst97 = (1960.0f / fConst0);
		fConst98 = (2352.0f / fConst0);
		fConst99 = (2744.0f / fConst0);
		fConst100 = (3136.0f / fConst0);
		fConst101 = (3528.0f / fConst0);
		fConst102 = (3920.0f / fConst0);
		fConst103 = (4312.0f / fConst0);
		fConst104 = (4704.0f / fConst0);
		fConst105 = (5096.0f / fConst0);
		fConst106 = (5488.0f / fConst0);
		fConst107 = (5880.0f / fConst0);
		float fConst108 = (9424.77832f / fConst0);
		fConst109 = ((1.0f - fConst108) / (fConst108 + 1.0f));
		float fConst110 = (1979.20337f / fConst0);
		fConst111 = ((1.0f - fConst110) / (fConst110 + 1.0f));
		float fConst112 = std::floor(((0.219990999f * fConst0) + 0.5f));
		float fConst113 = ((0.0f - (6.90775537f * fConst112)) / fConst0);
		float fConst114 = std::exp((0.5f * fConst113));
		float fConst115 = mydsp_faustpower2_f(fConst114);
		float fConst116 = std::cos((37699.1133f / fConst0));
		float fConst117 = (1.0f - (fConst115 * fConst116));
		float fConst118 = (1.0f - fConst115);
		float fConst119 = (fConst117 / fConst118);
		float fConst120 = std::sqrt(std::max<float>(0.0f, ((mydsp_faustpower2_f(fConst117) / mydsp_faustpower2_f(fConst118)) + -1.0f)));
		fConst121 = (fConst119 - fConst120);
		fConst122 = (fConst114 * (fConst120 + (1.0f - fConst119)));
		fConst123 = ((std::exp((0.333333343f * fConst113)) / fConst114) + -1.0f);
		float fConst124 = (1.0f / std::tan((628.318542f / fConst0)));
		fConst125 = (1.0f / (fConst124 + 1.0f));
		fConst126 = (1.0f - fConst124);
		float fConst127 = std::floor(((0.0191229992f * fConst0) + 0.5f));
		iConst128 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst112 - fConst127))));
		iConst129 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (0.0599999987f * fConst0))));
		iConst130 = int(std::min<float>(1024.0f, std::max<float>(0.0f, (fConst127 + -1.0f))));
		float fConst131 = std::floor(((0.256891012f * fConst0) + 0.5f));
		float fConst132 = ((0.0f - (6.90775537f * fConst131)) / fConst0);
		float fConst133 = std::exp((0.5f * fConst132));
		float fConst134 = mydsp_faustpower2_f(fConst133);
		float fConst135 = (1.0f - (fConst134 * fConst116));
		float fConst136 = (1.0f - fConst134);
		float fConst137 = (fConst135 / fConst136);
		float fConst138 = std::sqrt(std::max<float>(0.0f, ((mydsp_faustpower2_f(fConst135) / mydsp_faustpower2_f(fConst136)) + -1.0f)));
		fConst139 = (fConst137 - fConst138);
		fConst140 = (fConst133 * (fConst138 + (1.0f - fConst137)));
		fConst141 = ((std::exp((0.333333343f * fConst132)) / fConst133) + -1.0f);
		float fConst142 = std::floor(((0.0273330007f * fConst0) + 0.5f));
		iConst143 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst131 - fConst142))));
		iConst144 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst142 + -1.0f))));
		float fConst145 = std::floor(((0.192303002f * fConst0) + 0.5f));
		float fConst146 = ((0.0f - (6.90775537f * fConst145)) / fConst0);
		float fConst147 = std::exp((0.5f * fConst146));
		float fConst148 = mydsp_faustpower2_f(fConst147);
		float fConst149 = (1.0f - (fConst148 * fConst116));
		float fConst150 = (1.0f - fConst148);
		float fConst151 = (fConst149 / fConst150);
		float fConst152 = std::sqrt(std::max<float>(0.0f, ((mydsp_faustpower2_f(fConst149) / mydsp_faustpower2_f(fConst150)) + -1.0f)));
		fConst153 = (fConst151 - fConst152);
		fConst154 = (fConst147 * (fConst152 + (1.0f - fConst151)));
		fConst155 = ((std::exp((0.333333343f * fConst146)) / fConst147) + -1.0f);
		float fConst156 = std::floor(((0.0292910002f * fConst0) + 0.5f));
		iConst157 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst145 - fConst156))));
		iConst158 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst156 + -1.0f))));
		float fConst159 = std::floor(((0.210389003f * fConst0) + 0.5f));
		float fConst160 = ((0.0f - (6.90775537f * fConst159)) / fConst0);
		float fConst161 = std::exp((0.5f * fConst160));
		float fConst162 = mydsp_faustpower2_f(fConst161);
		float fConst163 = (1.0f - (fConst162 * fConst116));
		float fConst164 = (1.0f - fConst162);
		float fConst165 = (fConst163 / fConst164);
		float fConst166 = std::sqrt(std::max<float>(0.0f, ((mydsp_faustpower2_f(fConst163) / mydsp_faustpower2_f(fConst164)) + -1.0f)));
		fConst167 = (fConst165 - fConst166);
		fConst168 = (fConst161 * (fConst166 + (1.0f - fConst165)));
		fConst169 = ((std::exp((0.333333343f * fConst160)) / fConst161) + -1.0f);
		float fConst170 = std::floor(((0.0244210009f * fConst0) + 0.5f));
		iConst171 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst159 - fConst170))));
		iConst172 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst170 + -1.0f))));
		float fConst173 = std::floor(((0.125f * fConst0) + 0.5f));
		float fConst174 = ((0.0f - (6.90775537f * fConst173)) / fConst0);
		float fConst175 = std::exp((0.5f * fConst174));
		float fConst176 = mydsp_faustpower2_f(fConst175);
		float fConst177 = (1.0f - (fConst176 * fConst116));
		float fConst178 = (1.0f - fConst176);
		float fConst179 = (fConst177 / fConst178);
		float fConst180 = std::sqrt(std::max<float>(0.0f, ((mydsp_faustpower2_f(fConst177) / mydsp_faustpower2_f(fConst178)) + -1.0f)));
		fConst181 = (fConst179 - fConst180);
		fConst182 = (fConst175 * (fConst180 + (1.0f - fConst179)));
		fConst183 = ((std::exp((0.333333343f * fConst174)) / fConst175) + -1.0f);
		float fConst184 = std::floor(((0.0134579996f * fConst0) + 0.5f));
		iConst185 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst173 - fConst184))));
		iConst186 = int(std::min<float>(1024.0f, std::max<float>(0.0f, (fConst184 + -1.0f))));
		float fConst187 = std::floor(((0.127837002f * fConst0) + 0.5f));
		float fConst188 = ((0.0f - (6.90775537f * fConst187)) / fConst0);
		float fConst189 = std::exp((0.5f * fConst188));
		float fConst190 = mydsp_faustpower2_f(fConst189);
		float fConst191 = (1.0f - (fConst190 * fConst116));
		float fConst192 = (1.0f - fConst190);
		float fConst193 = (fConst191 / fConst192);
		float fConst194 = std::sqrt(std::max<float>(0.0f, ((mydsp_faustpower2_f(fConst191) / mydsp_faustpower2_f(fConst192)) + -1.0f)));
		fConst195 = (fConst193 - fConst194);
		fConst196 = (fConst189 * (fConst194 + (1.0f - fConst193)));
		fConst197 = ((std::exp((0.333333343f * fConst188)) / fConst189) + -1.0f);
		float fConst198 = std::floor(((0.0316039994f * fConst0) + 0.5f));
		iConst199 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst187 - fConst198))));
		iConst200 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst198 + -1.0f))));
		float fConst201 = std::floor(((0.174713001f * fConst0) + 0.5f));
		float fConst202 = ((0.0f - (6.90775537f * fConst201)) / fConst0);
		float fConst203 = std::exp((0.5f * fConst202));
		float fConst204 = mydsp_faustpower2_f(fConst203);
		float fConst205 = (1.0f - (fConst204 * fConst116));
		float fConst206 = (1.0f - fConst204);
		float fConst207 = (fConst205 / fConst206);
		float fConst208 = std::sqrt(std::max<float>(0.0f, ((mydsp_faustpower2_f(fConst205) / mydsp_faustpower2_f(fConst206)) + -1.0f)));
		fConst209 = (fConst207 - fConst208);
		fConst210 = (fConst203 * (fConst208 + (1.0f - fConst207)));
		fConst211 = ((std::exp((0.333333343f * fConst202)) / fConst203) + -1.0f);
		float fConst212 = std::floor(((0.0229039993f * fConst0) + 0.5f));
		iConst213 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst201 - fConst212))));
		iConst214 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst212 + -1.0f))));
		float fConst215 = std::floor(((0.153128996f * fConst0) + 0.5f));
		float fConst216 = ((0.0f - (6.90775537f * fConst215)) / fConst0);
		float fConst217 = std::exp((0.5f * fConst216));
		float fConst218 = mydsp_faustpower2_f(fConst217);
		float fConst219 = (1.0f - (fConst218 * fConst116));
		float fConst220 = (1.0f - fConst218);
		float fConst221 = (fConst219 / fConst220);
		float fConst222 = std::sqrt(std::max<float>(0.0f, ((mydsp_faustpower2_f(fConst219) / mydsp_faustpower2_f(fConst220)) + -1.0f)));
		fConst223 = (fConst221 - fConst222);
		fConst224 = (fConst217 * (fConst222 + (1.0f - fConst221)));
		fConst225 = ((std::exp((0.333333343f * fConst216)) / fConst217) + -1.0f);
		float fConst226 = std::floor(((0.0203460008f * fConst0) + 0.5f));
		iConst227 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst215 - fConst226))));
		iConst228 = int(std::min<float>(1024.0f, std::max<float>(0.0f, (fConst226 + -1.0f))));
		fConst229 = (0.0f - (std::cos(fConst110) * (fConst111 + 1.0f)));
		fConst230 = (0.0f - (std::cos(fConst108) * (fConst109 + 1.0f)));
	}
	
	virtual void instanceResetUserInterface() {
		fVslider0 = FAUSTFLOAT(-6.0f);
		fVslider1 = FAUSTFLOAT(0.0f);
		fButton0 = FAUSTFLOAT(0.0f);
		fHslider0 = FAUSTFLOAT(1.0f);
		fButton1 = FAUSTFLOAT(0.0f);
		fButton2 = FAUSTFLOAT(0.0f);
		fButton3 = FAUSTFLOAT(0.0f);
		fButton4 = FAUSTFLOAT(0.0f);
		fButton5 = FAUSTFLOAT(0.0f);
		fButton6 = FAUSTFLOAT(0.0f);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; (l0 < 2); l0 = (l0 + 1)) {
			fRec0[l0] = 0.0f;
		}
		for (int l1 = 0; (l1 < 2); l1 = (l1 + 1)) {
			fRec1[l1] = 0.0f;
		}
		for (int l2 = 0; (l2 < 2); l2 = (l2 + 1)) {
			fVec0[l2] = 0.0f;
		}
		for (int l3 = 0; (l3 < 2); l3 = (l3 + 1)) {
			iRec2[l3] = 0;
		}
		for (int l5 = 0; (l5 < 2); l5 = (l5 + 1)) {
			fRec4[l5] = 0.0f;
		}
		for (int l6 = 0; (l6 < 2); l6 = (l6 + 1)) {
			fRec5[l6] = 0.0f;
		}
		for (int l7 = 0; (l7 < 2); l7 = (l7 + 1)) {
			fRec6[l7] = 0.0f;
		}
		for (int l8 = 0; (l8 < 2); l8 = (l8 + 1)) {
			fRec7[l8] = 0.0f;
		}
		for (int l9 = 0; (l9 < 2); l9 = (l9 + 1)) {
			fRec8[l9] = 0.0f;
		}
		for (int l10 = 0; (l10 < 2); l10 = (l10 + 1)) {
			fRec9[l10] = 0.0f;
		}
		for (int l11 = 0; (l11 < 2); l11 = (l11 + 1)) {
			fRec10[l11] = 0.0f;
		}
		for (int l12 = 0; (l12 < 2); l12 = (l12 + 1)) {
			fRec11[l12] = 0.0f;
		}
		for (int l13 = 0; (l13 < 2); l13 = (l13 + 1)) {
			fRec12[l13] = 0.0f;
		}
		for (int l14 = 0; (l14 < 2); l14 = (l14 + 1)) {
			fRec13[l14] = 0.0f;
		}
		for (int l15 = 0; (l15 < 2); l15 = (l15 + 1)) {
			fRec14[l15] = 0.0f;
		}
		for (int l16 = 0; (l16 < 2); l16 = (l16 + 1)) {
			fRec15[l16] = 0.0f;
		}
		for (int l17 = 0; (l17 < 2); l17 = (l17 + 1)) {
			fRec16[l17] = 0.0f;
		}
		for (int l18 = 0; (l18 < 2); l18 = (l18 + 1)) {
			fRec17[l18] = 0.0f;
		}
		for (int l19 = 0; (l19 < 2); l19 = (l19 + 1)) {
			fRec18[l19] = 0.0f;
		}
		for (int l20 = 0; (l20 < 2); l20 = (l20 + 1)) {
			fVec1[l20] = 0.0f;
		}
		for (int l21 = 0; (l21 < 2); l21 = (l21 + 1)) {
			iRec19[l21] = 0;
		}
		for (int l22 = 0; (l22 < 2); l22 = (l22 + 1)) {
			fRec20[l22] = 0.0f;
		}
		for (int l23 = 0; (l23 < 2); l23 = (l23 + 1)) {
			fRec21[l23] = 0.0f;
		}
		for (int l24 = 0; (l24 < 2); l24 = (l24 + 1)) {
			fRec22[l24] = 0.0f;
		}
		for (int l25 = 0; (l25 < 2); l25 = (l25 + 1)) {
			fRec23[l25] = 0.0f;
		}
		for (int l26 = 0; (l26 < 2); l26 = (l26 + 1)) {
			fRec24[l26] = 0.0f;
		}
		for (int l27 = 0; (l27 < 2); l27 = (l27 + 1)) {
			fRec25[l27] = 0.0f;
		}
		for (int l28 = 0; (l28 < 2); l28 = (l28 + 1)) {
			fRec26[l28] = 0.0f;
		}
		for (int l29 = 0; (l29 < 2); l29 = (l29 + 1)) {
			fRec27[l29] = 0.0f;
		}
		for (int l30 = 0; (l30 < 2); l30 = (l30 + 1)) {
			fRec28[l30] = 0.0f;
		}
		for (int l31 = 0; (l31 < 2); l31 = (l31 + 1)) {
			fRec29[l31] = 0.0f;
		}
		for (int l32 = 0; (l32 < 2); l32 = (l32 + 1)) {
			fRec30[l32] = 0.0f;
		}
		for (int l33 = 0; (l33 < 2); l33 = (l33 + 1)) {
			fRec31[l33] = 0.0f;
		}
		for (int l34 = 0; (l34 < 2); l34 = (l34 + 1)) {
			fRec32[l34] = 0.0f;
		}
		for (int l35 = 0; (l35 < 2); l35 = (l35 + 1)) {
			fRec33[l35] = 0.0f;
		}
		for (int l36 = 0; (l36 < 2); l36 = (l36 + 1)) {
			fRec34[l36] = 0.0f;
		}
		for (int l37 = 0; (l37 < 2); l37 = (l37 + 1)) {
			fVec2[l37] = 0.0f;
		}
		for (int l38 = 0; (l38 < 2); l38 = (l38 + 1)) {
			iRec35[l38] = 0;
		}
		for (int l39 = 0; (l39 < 2); l39 = (l39 + 1)) {
			fRec36[l39] = 0.0f;
		}
		for (int l40 = 0; (l40 < 2); l40 = (l40 + 1)) {
			fRec37[l40] = 0.0f;
		}
		for (int l41 = 0; (l41 < 2); l41 = (l41 + 1)) {
			fRec38[l41] = 0.0f;
		}
		for (int l42 = 0; (l42 < 2); l42 = (l42 + 1)) {
			fRec39[l42] = 0.0f;
		}
		for (int l43 = 0; (l43 < 2); l43 = (l43 + 1)) {
			fRec40[l43] = 0.0f;
		}
		for (int l44 = 0; (l44 < 2); l44 = (l44 + 1)) {
			fRec41[l44] = 0.0f;
		}
		for (int l45 = 0; (l45 < 2); l45 = (l45 + 1)) {
			fRec42[l45] = 0.0f;
		}
		for (int l46 = 0; (l46 < 2); l46 = (l46 + 1)) {
			fRec43[l46] = 0.0f;
		}
		for (int l47 = 0; (l47 < 2); l47 = (l47 + 1)) {
			fRec44[l47] = 0.0f;
		}
		for (int l48 = 0; (l48 < 2); l48 = (l48 + 1)) {
			fRec45[l48] = 0.0f;
		}
		for (int l49 = 0; (l49 < 2); l49 = (l49 + 1)) {
			fRec46[l49] = 0.0f;
		}
		for (int l50 = 0; (l50 < 2); l50 = (l50 + 1)) {
			fRec47[l50] = 0.0f;
		}
		for (int l51 = 0; (l51 < 2); l51 = (l51 + 1)) {
			fRec48[l51] = 0.0f;
		}
		for (int l52 = 0; (l52 < 2); l52 = (l52 + 1)) {
			fRec49[l52] = 0.0f;
		}
		for (int l53 = 0; (l53 < 2); l53 = (l53 + 1)) {
			fRec50[l53] = 0.0f;
		}
		for (int l54 = 0; (l54 < 2); l54 = (l54 + 1)) {
			fVec3[l54] = 0.0f;
		}
		for (int l55 = 0; (l55 < 2); l55 = (l55 + 1)) {
			iRec51[l55] = 0;
		}
		for (int l56 = 0; (l56 < 2); l56 = (l56 + 1)) {
			fRec52[l56] = 0.0f;
		}
		for (int l57 = 0; (l57 < 2); l57 = (l57 + 1)) {
			fRec53[l57] = 0.0f;
		}
		for (int l58 = 0; (l58 < 2); l58 = (l58 + 1)) {
			fRec54[l58] = 0.0f;
		}
		for (int l59 = 0; (l59 < 2); l59 = (l59 + 1)) {
			fRec55[l59] = 0.0f;
		}
		for (int l60 = 0; (l60 < 2); l60 = (l60 + 1)) {
			fRec56[l60] = 0.0f;
		}
		for (int l61 = 0; (l61 < 2); l61 = (l61 + 1)) {
			fRec57[l61] = 0.0f;
		}
		for (int l62 = 0; (l62 < 2); l62 = (l62 + 1)) {
			fRec58[l62] = 0.0f;
		}
		for (int l63 = 0; (l63 < 2); l63 = (l63 + 1)) {
			fRec59[l63] = 0.0f;
		}
		for (int l64 = 0; (l64 < 2); l64 = (l64 + 1)) {
			fRec60[l64] = 0.0f;
		}
		for (int l65 = 0; (l65 < 2); l65 = (l65 + 1)) {
			fRec61[l65] = 0.0f;
		}
		for (int l66 = 0; (l66 < 2); l66 = (l66 + 1)) {
			fRec62[l66] = 0.0f;
		}
		for (int l67 = 0; (l67 < 2); l67 = (l67 + 1)) {
			fRec63[l67] = 0.0f;
		}
		for (int l68 = 0; (l68 < 2); l68 = (l68 + 1)) {
			fRec64[l68] = 0.0f;
		}
		for (int l69 = 0; (l69 < 2); l69 = (l69 + 1)) {
			fRec65[l69] = 0.0f;
		}
		for (int l70 = 0; (l70 < 2); l70 = (l70 + 1)) {
			fRec66[l70] = 0.0f;
		}
		for (int l71 = 0; (l71 < 2); l71 = (l71 + 1)) {
			fVec4[l71] = 0.0f;
		}
		for (int l72 = 0; (l72 < 2); l72 = (l72 + 1)) {
			iRec67[l72] = 0;
		}
		for (int l73 = 0; (l73 < 2); l73 = (l73 + 1)) {
			fRec68[l73] = 0.0f;
		}
		for (int l74 = 0; (l74 < 2); l74 = (l74 + 1)) {
			fRec69[l74] = 0.0f;
		}
		for (int l75 = 0; (l75 < 2); l75 = (l75 + 1)) {
			fRec70[l75] = 0.0f;
		}
		for (int l76 = 0; (l76 < 2); l76 = (l76 + 1)) {
			fRec71[l76] = 0.0f;
		}
		for (int l77 = 0; (l77 < 2); l77 = (l77 + 1)) {
			fRec72[l77] = 0.0f;
		}
		for (int l78 = 0; (l78 < 2); l78 = (l78 + 1)) {
			fRec73[l78] = 0.0f;
		}
		for (int l79 = 0; (l79 < 2); l79 = (l79 + 1)) {
			fRec74[l79] = 0.0f;
		}
		for (int l80 = 0; (l80 < 2); l80 = (l80 + 1)) {
			fRec75[l80] = 0.0f;
		}
		for (int l81 = 0; (l81 < 2); l81 = (l81 + 1)) {
			fRec76[l81] = 0.0f;
		}
		for (int l82 = 0; (l82 < 2); l82 = (l82 + 1)) {
			fRec77[l82] = 0.0f;
		}
		for (int l83 = 0; (l83 < 2); l83 = (l83 + 1)) {
			fRec78[l83] = 0.0f;
		}
		for (int l84 = 0; (l84 < 2); l84 = (l84 + 1)) {
			fRec79[l84] = 0.0f;
		}
		for (int l85 = 0; (l85 < 2); l85 = (l85 + 1)) {
			fRec80[l85] = 0.0f;
		}
		for (int l86 = 0; (l86 < 2); l86 = (l86 + 1)) {
			fRec81[l86] = 0.0f;
		}
		for (int l87 = 0; (l87 < 2); l87 = (l87 + 1)) {
			fRec82[l87] = 0.0f;
		}
		for (int l88 = 0; (l88 < 2); l88 = (l88 + 1)) {
			fVec5[l88] = 0.0f;
		}
		for (int l89 = 0; (l89 < 2); l89 = (l89 + 1)) {
			iRec83[l89] = 0;
		}
		for (int l90 = 0; (l90 < 2); l90 = (l90 + 1)) {
			fRec84[l90] = 0.0f;
		}
		for (int l91 = 0; (l91 < 2); l91 = (l91 + 1)) {
			fRec85[l91] = 0.0f;
		}
		for (int l92 = 0; (l92 < 2); l92 = (l92 + 1)) {
			fRec86[l92] = 0.0f;
		}
		for (int l93 = 0; (l93 < 2); l93 = (l93 + 1)) {
			fRec87[l93] = 0.0f;
		}
		for (int l94 = 0; (l94 < 2); l94 = (l94 + 1)) {
			fRec88[l94] = 0.0f;
		}
		for (int l95 = 0; (l95 < 2); l95 = (l95 + 1)) {
			fRec89[l95] = 0.0f;
		}
		for (int l96 = 0; (l96 < 2); l96 = (l96 + 1)) {
			fRec90[l96] = 0.0f;
		}
		for (int l97 = 0; (l97 < 2); l97 = (l97 + 1)) {
			fRec91[l97] = 0.0f;
		}
		for (int l98 = 0; (l98 < 2); l98 = (l98 + 1)) {
			fRec92[l98] = 0.0f;
		}
		for (int l99 = 0; (l99 < 2); l99 = (l99 + 1)) {
			fRec93[l99] = 0.0f;
		}
		for (int l100 = 0; (l100 < 2); l100 = (l100 + 1)) {
			fRec94[l100] = 0.0f;
		}
		for (int l101 = 0; (l101 < 2); l101 = (l101 + 1)) {
			fRec95[l101] = 0.0f;
		}
		for (int l102 = 0; (l102 < 2); l102 = (l102 + 1)) {
			fRec96[l102] = 0.0f;
		}
		for (int l103 = 0; (l103 < 2); l103 = (l103 + 1)) {
			fRec97[l103] = 0.0f;
		}
		for (int l104 = 0; (l104 < 2); l104 = (l104 + 1)) {
			fRec98[l104] = 0.0f;
		}
		for (int l105 = 0; (l105 < 2); l105 = (l105 + 1)) {
			fVec6[l105] = 0.0f;
		}
		for (int l106 = 0; (l106 < 2); l106 = (l106 + 1)) {
			iRec99[l106] = 0;
		}
		for (int l107 = 0; (l107 < 2); l107 = (l107 + 1)) {
			fRec100[l107] = 0.0f;
		}
		for (int l108 = 0; (l108 < 2); l108 = (l108 + 1)) {
			fRec101[l108] = 0.0f;
		}
		for (int l109 = 0; (l109 < 2); l109 = (l109 + 1)) {
			fRec102[l109] = 0.0f;
		}
		for (int l110 = 0; (l110 < 2); l110 = (l110 + 1)) {
			fRec103[l110] = 0.0f;
		}
		for (int l111 = 0; (l111 < 2); l111 = (l111 + 1)) {
			fRec104[l111] = 0.0f;
		}
		for (int l112 = 0; (l112 < 2); l112 = (l112 + 1)) {
			fRec105[l112] = 0.0f;
		}
		for (int l113 = 0; (l113 < 2); l113 = (l113 + 1)) {
			fRec106[l113] = 0.0f;
		}
		for (int l114 = 0; (l114 < 2); l114 = (l114 + 1)) {
			fRec107[l114] = 0.0f;
		}
		for (int l115 = 0; (l115 < 2); l115 = (l115 + 1)) {
			fRec108[l115] = 0.0f;
		}
		for (int l116 = 0; (l116 < 2); l116 = (l116 + 1)) {
			fRec109[l116] = 0.0f;
		}
		for (int l117 = 0; (l117 < 2); l117 = (l117 + 1)) {
			fRec110[l117] = 0.0f;
		}
		for (int l118 = 0; (l118 < 2); l118 = (l118 + 1)) {
			fRec111[l118] = 0.0f;
		}
		for (int l119 = 0; (l119 < 2); l119 = (l119 + 1)) {
			fRec112[l119] = 0.0f;
		}
		for (int l120 = 0; (l120 < 2); l120 = (l120 + 1)) {
			fRec113[l120] = 0.0f;
		}
		for (int l121 = 0; (l121 < 2); l121 = (l121 + 1)) {
			fRec114[l121] = 0.0f;
		}
		for (int l122 = 0; (l122 < 2); l122 = (l122 + 1)) {
			fRec128[l122] = 0.0f;
		}
		for (int l123 = 0; (l123 < 2); l123 = (l123 + 1)) {
			fRec127[l123] = 0.0f;
		}
		IOTA = 0;
		for (int l124 = 0; (l124 < 32768); l124 = (l124 + 1)) {
			fVec7[l124] = 0.0f;
		}
		for (int l125 = 0; (l125 < 16384); l125 = (l125 + 1)) {
			fVec8[l125] = 0.0f;
		}
		for (int l126 = 0; (l126 < 2048); l126 = (l126 + 1)) {
			fVec9[l126] = 0.0f;
		}
		for (int l127 = 0; (l127 < 2); l127 = (l127 + 1)) {
			fRec125[l127] = 0.0f;
		}
		for (int l128 = 0; (l128 < 2); l128 = (l128 + 1)) {
			fRec132[l128] = 0.0f;
		}
		for (int l129 = 0; (l129 < 2); l129 = (l129 + 1)) {
			fRec131[l129] = 0.0f;
		}
		for (int l130 = 0; (l130 < 32768); l130 = (l130 + 1)) {
			fVec10[l130] = 0.0f;
		}
		for (int l131 = 0; (l131 < 4096); l131 = (l131 + 1)) {
			fVec11[l131] = 0.0f;
		}
		for (int l132 = 0; (l132 < 2); l132 = (l132 + 1)) {
			fRec129[l132] = 0.0f;
		}
		for (int l133 = 0; (l133 < 2); l133 = (l133 + 1)) {
			fRec136[l133] = 0.0f;
		}
		for (int l134 = 0; (l134 < 2); l134 = (l134 + 1)) {
			fRec135[l134] = 0.0f;
		}
		for (int l135 = 0; (l135 < 16384); l135 = (l135 + 1)) {
			fVec12[l135] = 0.0f;
		}
		for (int l136 = 0; (l136 < 4096); l136 = (l136 + 1)) {
			fVec13[l136] = 0.0f;
		}
		for (int l137 = 0; (l137 < 2); l137 = (l137 + 1)) {
			fRec133[l137] = 0.0f;
		}
		for (int l138 = 0; (l138 < 2); l138 = (l138 + 1)) {
			fRec140[l138] = 0.0f;
		}
		for (int l139 = 0; (l139 < 2); l139 = (l139 + 1)) {
			fRec139[l139] = 0.0f;
		}
		for (int l140 = 0; (l140 < 32768); l140 = (l140 + 1)) {
			fVec14[l140] = 0.0f;
		}
		for (int l141 = 0; (l141 < 4096); l141 = (l141 + 1)) {
			fVec15[l141] = 0.0f;
		}
		for (int l142 = 0; (l142 < 2); l142 = (l142 + 1)) {
			fRec137[l142] = 0.0f;
		}
		for (int l143 = 0; (l143 < 2); l143 = (l143 + 1)) {
			fRec144[l143] = 0.0f;
		}
		for (int l144 = 0; (l144 < 2); l144 = (l144 + 1)) {
			fRec143[l144] = 0.0f;
		}
		for (int l145 = 0; (l145 < 16384); l145 = (l145 + 1)) {
			fVec16[l145] = 0.0f;
		}
		for (int l146 = 0; (l146 < 2048); l146 = (l146 + 1)) {
			fVec17[l146] = 0.0f;
		}
		for (int l147 = 0; (l147 < 2); l147 = (l147 + 1)) {
			fRec141[l147] = 0.0f;
		}
		for (int l148 = 0; (l148 < 2); l148 = (l148 + 1)) {
			fRec148[l148] = 0.0f;
		}
		for (int l149 = 0; (l149 < 2); l149 = (l149 + 1)) {
			fRec147[l149] = 0.0f;
		}
		for (int l150 = 0; (l150 < 16384); l150 = (l150 + 1)) {
			fVec18[l150] = 0.0f;
		}
		for (int l151 = 0; (l151 < 4096); l151 = (l151 + 1)) {
			fVec19[l151] = 0.0f;
		}
		for (int l152 = 0; (l152 < 2); l152 = (l152 + 1)) {
			fRec145[l152] = 0.0f;
		}
		for (int l153 = 0; (l153 < 2); l153 = (l153 + 1)) {
			fRec152[l153] = 0.0f;
		}
		for (int l154 = 0; (l154 < 2); l154 = (l154 + 1)) {
			fRec151[l154] = 0.0f;
		}
		for (int l155 = 0; (l155 < 16384); l155 = (l155 + 1)) {
			fVec20[l155] = 0.0f;
		}
		for (int l156 = 0; (l156 < 4096); l156 = (l156 + 1)) {
			fVec21[l156] = 0.0f;
		}
		for (int l157 = 0; (l157 < 2); l157 = (l157 + 1)) {
			fRec149[l157] = 0.0f;
		}
		for (int l158 = 0; (l158 < 2); l158 = (l158 + 1)) {
			fRec156[l158] = 0.0f;
		}
		for (int l159 = 0; (l159 < 2); l159 = (l159 + 1)) {
			fRec155[l159] = 0.0f;
		}
		for (int l160 = 0; (l160 < 16384); l160 = (l160 + 1)) {
			fVec22[l160] = 0.0f;
		}
		for (int l161 = 0; (l161 < 2048); l161 = (l161 + 1)) {
			fVec23[l161] = 0.0f;
		}
		for (int l162 = 0; (l162 < 2); l162 = (l162 + 1)) {
			fRec153[l162] = 0.0f;
		}
		for (int l163 = 0; (l163 < 3); l163 = (l163 + 1)) {
			fRec117[l163] = 0.0f;
		}
		for (int l164 = 0; (l164 < 3); l164 = (l164 + 1)) {
			fRec118[l164] = 0.0f;
		}
		for (int l165 = 0; (l165 < 3); l165 = (l165 + 1)) {
			fRec119[l165] = 0.0f;
		}
		for (int l166 = 0; (l166 < 3); l166 = (l166 + 1)) {
			fRec120[l166] = 0.0f;
		}
		for (int l167 = 0; (l167 < 3); l167 = (l167 + 1)) {
			fRec121[l167] = 0.0f;
		}
		for (int l168 = 0; (l168 < 3); l168 = (l168 + 1)) {
			fRec122[l168] = 0.0f;
		}
		for (int l169 = 0; (l169 < 3); l169 = (l169 + 1)) {
			fRec123[l169] = 0.0f;
		}
		for (int l170 = 0; (l170 < 3); l170 = (l170 + 1)) {
			fRec124[l170] = 0.0f;
		}
		for (int l171 = 0; (l171 < 3); l171 = (l171 + 1)) {
			fRec116[l171] = 0.0f;
		}
		for (int l172 = 0; (l172 < 3); l172 = (l172 + 1)) {
			fRec115[l172] = 0.0f;
		}
		for (int l173 = 0; (l173 < 3); l173 = (l173 + 1)) {
			fRec158[l173] = 0.0f;
		}
		for (int l174 = 0; (l174 < 3); l174 = (l174 + 1)) {
			fRec157[l174] = 0.0f;
		}
	}
	
	virtual void init(int sample_rate) {
		classInit(sample_rate);
		instanceInit(sample_rate);
	}
	virtual void instanceInit(int sample_rate) {
		instanceConstants(sample_rate);
		instanceResetUserInterface();
		instanceClear();
	}
	
	virtual mydsp* clone() {
		return new mydsp();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("LydNY22");
		ui_interface->addButton("A", &fButton0);
		ui_interface->addButton("B", &fButton1);
		ui_interface->addButton("C", &fButton2);
		ui_interface->addButton("D", &fButton3);
		ui_interface->addButton("E", &fButton4);
		ui_interface->addButton("F", &fButton5);
		ui_interface->addButton("G", &fButton6);
		ui_interface->openHorizontalBox("Zita Light");
		ui_interface->declare(&fVslider1, "1", "");
		ui_interface->declare(&fVslider1, "style", "knob");
		ui_interface->declare(&fVslider1, "tooltip", "-1 = dry, 1 = wet");
		ui_interface->addVerticalSlider("Dry/Wet Mix", &fVslider1, 0.0f, -1.0f, 1.0f, 0.00999999978f);
		ui_interface->declare(&fVslider0, "2", "");
		ui_interface->declare(&fVslider0, "style", "knob");
		ui_interface->declare(&fVslider0, "tooltip", "Output scale   factor");
		ui_interface->declare(&fVslider0, "unit", "dB");
		ui_interface->addVerticalSlider("Level", &fVslider0, -6.0f, -70.0f, 40.0f, 0.100000001f);
		ui_interface->closeBox();
		ui_interface->addHorizontalSlider("rel", &fHslider0, 1.0f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) {
		FAUSTFLOAT* output0 = outputs[0];
		FAUSTFLOAT* output1 = outputs[1];
		float fSlow0 = (0.00100000005f * std::pow(10.0f, (0.0500000007f * float(fVslider0))));
		float fSlow1 = (0.00100000005f * float(fVslider1));
		float fSlow2 = float(fButton0);
		float fSlow3 = (1.0f / std::max<float>(1.0f, (fConst0 * float(fHslider0))));
		float fSlow4 = float(fButton1);
		float fSlow5 = float(fButton2);
		float fSlow6 = float(fButton3);
		float fSlow7 = float(fButton4);
		float fSlow8 = float(fButton5);
		float fSlow9 = float(fButton6);
		for (int i = 0; (i < count); i = (i + 1)) {
			fRec0[0] = (fSlow0 + (0.999000013f * fRec0[1]));
			fRec1[0] = (fSlow1 + (0.999000013f * fRec1[1]));
			float fTemp0 = (fRec1[0] + 1.0f);
			fVec0[0] = fSlow2;
			iRec2[0] = (((iRec2[1] + (iRec2[1] > 0)) * (fSlow2 <= fVec0[1])) + (fSlow2 > fVec0[1]));
			float fTemp1 = float(iRec2[0]);
			fRec4[0] = (fConst3 + (fRec4[1] - std::floor((fConst3 + fRec4[1]))));
			fRec5[0] = (fConst4 + (fRec5[1] - std::floor((fConst4 + fRec5[1]))));
			fRec6[0] = (fConst5 + (fRec6[1] - std::floor((fConst5 + fRec6[1]))));
			fRec7[0] = (fConst6 + (fRec7[1] - std::floor((fConst6 + fRec7[1]))));
			fRec8[0] = (fConst7 + (fRec8[1] - std::floor((fConst7 + fRec8[1]))));
			fRec9[0] = (fConst8 + (fRec9[1] - std::floor((fConst8 + fRec9[1]))));
			fRec10[0] = (fConst9 + (fRec10[1] - std::floor((fConst9 + fRec10[1]))));
			fRec11[0] = (fConst10 + (fRec11[1] - std::floor((fConst10 + fRec11[1]))));
			fRec12[0] = (fConst11 + (fRec12[1] - std::floor((fConst11 + fRec12[1]))));
			fRec13[0] = (fConst12 + (fRec13[1] - std::floor((fConst12 + fRec13[1]))));
			fRec14[0] = (fConst13 + (fRec14[1] - std::floor((fConst13 + fRec14[1]))));
			fRec15[0] = (fConst14 + (fRec15[1] - std::floor((fConst14 + fRec15[1]))));
			fRec16[0] = (fConst15 + (fRec16[1] - std::floor((fConst15 + fRec16[1]))));
			fRec17[0] = (fConst16 + (fRec17[1] - std::floor((fConst16 + fRec17[1]))));
			fRec18[0] = (fConst17 + (fRec18[1] - std::floor((fConst17 + fRec18[1]))));
			fVec1[0] = fSlow4;
			iRec19[0] = (((iRec19[1] + (iRec19[1] > 0)) * (fSlow4 <= fVec1[1])) + (fSlow4 > fVec1[1]));
			float fTemp2 = float(iRec19[0]);
			fRec20[0] = (fConst18 + (fRec20[1] - std::floor((fConst18 + fRec20[1]))));
			fRec21[0] = (fConst19 + (fRec21[1] - std::floor((fConst19 + fRec21[1]))));
			fRec22[0] = (fConst20 + (fRec22[1] - std::floor((fConst20 + fRec22[1]))));
			fRec23[0] = (fConst21 + (fRec23[1] - std::floor((fConst21 + fRec23[1]))));
			fRec24[0] = (fConst22 + (fRec24[1] - std::floor((fConst22 + fRec24[1]))));
			fRec25[0] = (fConst23 + (fRec25[1] - std::floor((fConst23 + fRec25[1]))));
			fRec26[0] = (fConst24 + (fRec26[1] - std::floor((fConst24 + fRec26[1]))));
			fRec27[0] = (fConst25 + (fRec27[1] - std::floor((fConst25 + fRec27[1]))));
			fRec28[0] = (fConst26 + (fRec28[1] - std::floor((fConst26 + fRec28[1]))));
			fRec29[0] = (fConst27 + (fRec29[1] - std::floor((fConst27 + fRec29[1]))));
			fRec30[0] = (fConst28 + (fRec30[1] - std::floor((fConst28 + fRec30[1]))));
			fRec31[0] = (fConst29 + (fRec31[1] - std::floor((fConst29 + fRec31[1]))));
			fRec32[0] = (fConst30 + (fRec32[1] - std::floor((fConst30 + fRec32[1]))));
			fRec33[0] = (fConst31 + (fRec33[1] - std::floor((fConst31 + fRec33[1]))));
			fRec34[0] = (fConst32 + (fRec34[1] - std::floor((fConst32 + fRec34[1]))));
			fVec2[0] = fSlow5;
			iRec35[0] = (((iRec35[1] + (iRec35[1] > 0)) * (fSlow5 <= fVec2[1])) + (fSlow5 > fVec2[1]));
			float fTemp3 = float(iRec35[0]);
			fRec36[0] = (fConst33 + (fRec36[1] - std::floor((fConst33 + fRec36[1]))));
			fRec37[0] = (fConst34 + (fRec37[1] - std::floor((fConst34 + fRec37[1]))));
			fRec38[0] = (fConst35 + (fRec38[1] - std::floor((fConst35 + fRec38[1]))));
			fRec39[0] = (fConst36 + (fRec39[1] - std::floor((fConst36 + fRec39[1]))));
			fRec40[0] = (fConst37 + (fRec40[1] - std::floor((fConst37 + fRec40[1]))));
			fRec41[0] = (fConst38 + (fRec41[1] - std::floor((fConst38 + fRec41[1]))));
			fRec42[0] = (fConst39 + (fRec42[1] - std::floor((fConst39 + fRec42[1]))));
			fRec43[0] = (fConst40 + (fRec43[1] - std::floor((fConst40 + fRec43[1]))));
			fRec44[0] = (fConst41 + (fRec44[1] - std::floor((fConst41 + fRec44[1]))));
			fRec45[0] = (fConst42 + (fRec45[1] - std::floor((fConst42 + fRec45[1]))));
			fRec46[0] = (fConst43 + (fRec46[1] - std::floor((fConst43 + fRec46[1]))));
			fRec47[0] = (fConst44 + (fRec47[1] - std::floor((fConst44 + fRec47[1]))));
			fRec48[0] = (fConst45 + (fRec48[1] - std::floor((fConst45 + fRec48[1]))));
			fRec49[0] = (fConst46 + (fRec49[1] - std::floor((fConst46 + fRec49[1]))));
			fRec50[0] = (fConst47 + (fRec50[1] - std::floor((fConst47 + fRec50[1]))));
			fVec3[0] = fSlow6;
			iRec51[0] = (((iRec51[1] + (iRec51[1] > 0)) * (fSlow6 <= fVec3[1])) + (fSlow6 > fVec3[1]));
			float fTemp4 = float(iRec51[0]);
			fRec52[0] = (fConst48 + (fRec52[1] - std::floor((fConst48 + fRec52[1]))));
			fRec53[0] = (fConst49 + (fRec53[1] - std::floor((fConst49 + fRec53[1]))));
			fRec54[0] = (fConst50 + (fRec54[1] - std::floor((fConst50 + fRec54[1]))));
			fRec55[0] = (fConst51 + (fRec55[1] - std::floor((fConst51 + fRec55[1]))));
			fRec56[0] = (fConst52 + (fRec56[1] - std::floor((fConst52 + fRec56[1]))));
			fRec57[0] = (fConst53 + (fRec57[1] - std::floor((fConst53 + fRec57[1]))));
			fRec58[0] = (fConst54 + (fRec58[1] - std::floor((fConst54 + fRec58[1]))));
			fRec59[0] = (fConst55 + (fRec59[1] - std::floor((fConst55 + fRec59[1]))));
			fRec60[0] = (fConst56 + (fRec60[1] - std::floor((fConst56 + fRec60[1]))));
			fRec61[0] = (fConst57 + (fRec61[1] - std::floor((fConst57 + fRec61[1]))));
			fRec62[0] = (fConst58 + (fRec62[1] - std::floor((fConst58 + fRec62[1]))));
			fRec63[0] = (fConst59 + (fRec63[1] - std::floor((fConst59 + fRec63[1]))));
			fRec64[0] = (fConst60 + (fRec64[1] - std::floor((fConst60 + fRec64[1]))));
			fRec65[0] = (fConst61 + (fRec65[1] - std::floor((fConst61 + fRec65[1]))));
			fRec66[0] = (fConst62 + (fRec66[1] - std::floor((fConst62 + fRec66[1]))));
			fVec4[0] = fSlow7;
			iRec67[0] = (((iRec67[1] + (iRec67[1] > 0)) * (fSlow7 <= fVec4[1])) + (fSlow7 > fVec4[1]));
			float fTemp5 = float(iRec67[0]);
			fRec68[0] = (fConst63 + (fRec68[1] - std::floor((fConst63 + fRec68[1]))));
			fRec69[0] = (fConst64 + (fRec69[1] - std::floor((fConst64 + fRec69[1]))));
			fRec70[0] = (fConst65 + (fRec70[1] - std::floor((fConst65 + fRec70[1]))));
			fRec71[0] = (fConst66 + (fRec71[1] - std::floor((fConst66 + fRec71[1]))));
			fRec72[0] = (fConst67 + (fRec72[1] - std::floor((fConst67 + fRec72[1]))));
			fRec73[0] = (fConst68 + (fRec73[1] - std::floor((fConst68 + fRec73[1]))));
			fRec74[0] = (fConst69 + (fRec74[1] - std::floor((fConst69 + fRec74[1]))));
			fRec75[0] = (fConst70 + (fRec75[1] - std::floor((fConst70 + fRec75[1]))));
			fRec76[0] = (fConst71 + (fRec76[1] - std::floor((fConst71 + fRec76[1]))));
			fRec77[0] = (fConst72 + (fRec77[1] - std::floor((fConst72 + fRec77[1]))));
			fRec78[0] = (fConst73 + (fRec78[1] - std::floor((fConst73 + fRec78[1]))));
			fRec79[0] = (fConst74 + (fRec79[1] - std::floor((fConst74 + fRec79[1]))));
			fRec80[0] = (fConst75 + (fRec80[1] - std::floor((fConst75 + fRec80[1]))));
			fRec81[0] = (fConst76 + (fRec81[1] - std::floor((fConst76 + fRec81[1]))));
			fRec82[0] = (fConst77 + (fRec82[1] - std::floor((fConst77 + fRec82[1]))));
			fVec5[0] = fSlow8;
			iRec83[0] = (((iRec83[1] + (iRec83[1] > 0)) * (fSlow8 <= fVec5[1])) + (fSlow8 > fVec5[1]));
			float fTemp6 = float(iRec83[0]);
			fRec84[0] = (fConst78 + (fRec84[1] - std::floor((fConst78 + fRec84[1]))));
			fRec85[0] = (fConst79 + (fRec85[1] - std::floor((fConst79 + fRec85[1]))));
			fRec86[0] = (fConst80 + (fRec86[1] - std::floor((fConst80 + fRec86[1]))));
			fRec87[0] = (fConst81 + (fRec87[1] - std::floor((fConst81 + fRec87[1]))));
			fRec88[0] = (fConst82 + (fRec88[1] - std::floor((fConst82 + fRec88[1]))));
			fRec89[0] = (fConst83 + (fRec89[1] - std::floor((fConst83 + fRec89[1]))));
			fRec90[0] = (fConst84 + (fRec90[1] - std::floor((fConst84 + fRec90[1]))));
			fRec91[0] = (fConst85 + (fRec91[1] - std::floor((fConst85 + fRec91[1]))));
			fRec92[0] = (fConst86 + (fRec92[1] - std::floor((fConst86 + fRec92[1]))));
			fRec93[0] = (fConst87 + (fRec93[1] - std::floor((fConst87 + fRec93[1]))));
			fRec94[0] = (fConst88 + (fRec94[1] - std::floor((fConst88 + fRec94[1]))));
			fRec95[0] = (fConst89 + (fRec95[1] - std::floor((fConst89 + fRec95[1]))));
			fRec96[0] = (fConst90 + (fRec96[1] - std::floor((fConst90 + fRec96[1]))));
			fRec97[0] = (fConst91 + (fRec97[1] - std::floor((fConst91 + fRec97[1]))));
			fRec98[0] = (fConst92 + (fRec98[1] - std::floor((fConst92 + fRec98[1]))));
			fVec6[0] = fSlow9;
			iRec99[0] = (((iRec99[1] + (iRec99[1] > 0)) * (fSlow9 <= fVec6[1])) + (fSlow9 > fVec6[1]));
			float fTemp7 = float(iRec99[0]);
			fRec100[0] = (fConst93 + (fRec100[1] - std::floor((fConst93 + fRec100[1]))));
			fRec101[0] = (fConst94 + (fRec101[1] - std::floor((fConst94 + fRec101[1]))));
			fRec102[0] = (fConst95 + (fRec102[1] - std::floor((fConst95 + fRec102[1]))));
			fRec103[0] = (fConst96 + (fRec103[1] - std::floor((fConst96 + fRec103[1]))));
			fRec104[0] = (fConst97 + (fRec104[1] - std::floor((fConst97 + fRec104[1]))));
			fRec105[0] = (fConst98 + (fRec105[1] - std::floor((fConst98 + fRec105[1]))));
			fRec106[0] = (fConst99 + (fRec106[1] - std::floor((fConst99 + fRec106[1]))));
			fRec107[0] = (fConst100 + (fRec107[1] - std::floor((fConst100 + fRec107[1]))));
			fRec108[0] = (fConst101 + (fRec108[1] - std::floor((fConst101 + fRec108[1]))));
			fRec109[0] = (fConst102 + (fRec109[1] - std::floor((fConst102 + fRec109[1]))));
			fRec110[0] = (fConst103 + (fRec110[1] - std::floor((fConst103 + fRec110[1]))));
			fRec111[0] = (fConst104 + (fRec111[1] - std::floor((fConst104 + fRec111[1]))));
			fRec112[0] = (fConst105 + (fRec112[1] - std::floor((fConst105 + fRec112[1]))));
			fRec113[0] = (fConst106 + (fRec113[1] - std::floor((fConst106 + fRec113[1]))));
			fRec114[0] = (fConst107 + (fRec114[1] - std::floor((fConst107 + fRec114[1]))));
			float fTemp8 = std::max<float>(-1.0f, std::min<float>(1.0f, ((0.837295473f * ((std::max<float>(0.0f, std::min<float>((fConst2 * fTemp1), ((fSlow3 * (fConst1 - fTemp1)) + 1.0f))) * (((((((((((((((0.5f * ftbl0mydspSIG0[int((65536.0f * fRec4[0]))]) + (0.25f * ftbl0mydspSIG0[int((65536.0f * fRec5[0]))])) + (0.125f * ftbl0mydspSIG0[int((65536.0f * fRec6[0]))])) + (0.0625f * ftbl0mydspSIG0[int((65536.0f * fRec7[0]))])) + (0.0317000002f * ftbl0mydspSIG0[int((65536.0f * fRec8[0]))])) + (0.0149999997f * ftbl0mydspSIG0[int((65536.0f * fRec9[0]))])) + (0.00749999983f * ftbl0mydspSIG0[int((65536.0f * fRec10[0]))])) + (0.00374999992f * ftbl0mydspSIG0[int((65536.0f * fRec11[0]))])) + (0.00187499996f * ftbl0mydspSIG0[int((65536.0f * fRec12[0]))])) + (0.000937499979f * ftbl0mydspSIG0[int((65536.0f * fRec13[0]))])) + (0.00046874999f * ftbl0mydspSIG0[int((65536.0f * fRec14[0]))])) + (0.000234380001f * ftbl0mydspSIG0[int((65536.0f * fRec15[0]))])) + (0.00011719f * ftbl0mydspSIG0[int((65536.0f * fRec16[0]))])) + (5.85999987e-05f * ftbl0mydspSIG0[int((65536.0f * fRec17[0]))])) + (2.92999994e-05f * ftbl0mydspSIG0[int((65536.0f * fRec18[0]))]))) + ((((((std::max<float>(0.0f, std::min<float>((fConst2 * fTemp2), ((fSlow3 * (fConst1 - fTemp2)) + 1.0f))) * (((((((((((((((0.5f * ftbl0mydspSIG0[int((65536.0f * fRec20[0]))]) + (0.25f * ftbl0mydspSIG0[int((65536.0f * fRec21[0]))])) + (0.125f * ftbl0mydspSIG0[int((65536.0f * fRec22[0]))])) + (0.0625f * ftbl0mydspSIG0[int((65536.0f * fRec23[0]))])) + (0.0317000002f * ftbl0mydspSIG0[int((65536.0f * fRec24[0]))])) + (0.0149999997f * ftbl0mydspSIG0[int((65536.0f * fRec25[0]))])) + (0.00749999983f * ftbl0mydspSIG0[int((65536.0f * fRec26[0]))])) + (0.00374999992f * ftbl0mydspSIG0[int((65536.0f * fRec27[0]))])) + (0.00187499996f * ftbl0mydspSIG0[int((65536.0f * fRec28[0]))])) + (0.000937499979f * ftbl0mydspSIG0[int((65536.0f * fRec29[0]))])) + (0.00046874999f * ftbl0mydspSIG0[int((65536.0f * fRec30[0]))])) + (0.000234380001f * ftbl0mydspSIG0[int((65536.0f * fRec31[0]))])) + (0.00011719f * ftbl0mydspSIG0[int((65536.0f * fRec32[0]))])) + (5.85999987e-05f * ftbl0mydspSIG0[int((65536.0f * fRec33[0]))])) + (2.92999994e-05f * ftbl0mydspSIG0[int((65536.0f * fRec34[0]))]))) + (std::max<float>(0.0f, std::min<float>((fConst2 * fTemp3), ((fSlow3 * (fConst1 - fTemp3)) + 1.0f))) * (((((((((((((((0.5f * ftbl0mydspSIG0[int((65536.0f * fRec36[0]))]) + (0.25f * ftbl0mydspSIG0[int((65536.0f * fRec37[0]))])) + (0.125f * ftbl0mydspSIG0[int((65536.0f * fRec38[0]))])) + (0.0625f * ftbl0mydspSIG0[int((65536.0f * fRec39[0]))])) + (0.0317000002f * ftbl0mydspSIG0[int((65536.0f * fRec40[0]))])) + (0.0149999997f * ftbl0mydspSIG0[int((65536.0f * fRec41[0]))])) + (0.00749999983f * ftbl0mydspSIG0[int((65536.0f * fRec42[0]))])) + (0.00374999992f * ftbl0mydspSIG0[int((65536.0f * fRec43[0]))])) + (0.00187499996f * ftbl0mydspSIG0[int((65536.0f * fRec44[0]))])) + (0.000937499979f * ftbl0mydspSIG0[int((65536.0f * fRec45[0]))])) + (0.00046874999f * ftbl0mydspSIG0[int((65536.0f * fRec46[0]))])) + (0.000234380001f * ftbl0mydspSIG0[int((65536.0f * fRec47[0]))])) + (0.00011719f * ftbl0mydspSIG0[int((65536.0f * fRec48[0]))])) + (5.85999987e-05f * ftbl0mydspSIG0[int((65536.0f * fRec49[0]))])) + (2.92999994e-05f * ftbl0mydspSIG0[int((65536.0f * fRec50[0]))])))) + (std::max<float>(0.0f, std::min<float>((fConst2 * fTemp4), ((fSlow3 * (fConst1 - fTemp4)) + 1.0f))) * (((((((((((((((0.5f * ftbl0mydspSIG0[int((65536.0f * fRec52[0]))]) + (0.25f * ftbl0mydspSIG0[int((65536.0f * fRec53[0]))])) + (0.125f * ftbl0mydspSIG0[int((65536.0f * fRec54[0]))])) + (0.0625f * ftbl0mydspSIG0[int((65536.0f * fRec55[0]))])) + (0.0317000002f * ftbl0mydspSIG0[int((65536.0f * fRec56[0]))])) + (0.0149999997f * ftbl0mydspSIG0[int((65536.0f * fRec57[0]))])) + (0.00749999983f * ftbl0mydspSIG0[int((65536.0f * fRec58[0]))])) + (0.00374999992f * ftbl0mydspSIG0[int((65536.0f * fRec59[0]))])) + (0.00187499996f * ftbl0mydspSIG0[int((65536.0f * fRec60[0]))])) + (0.000937499979f * ftbl0mydspSIG0[int((65536.0f * fRec61[0]))])) + (0.00046874999f * ftbl0mydspSIG0[int((65536.0f * fRec62[0]))])) + (0.000234380001f * ftbl0mydspSIG0[int((65536.0f * fRec63[0]))])) + (0.00011719f * ftbl0mydspSIG0[int((65536.0f * fRec64[0]))])) + (5.85999987e-05f * ftbl0mydspSIG0[int((65536.0f * fRec65[0]))])) + (2.92999994e-05f * ftbl0mydspSIG0[int((65536.0f * fRec66[0]))])))) + (std::max<float>(0.0f, std::min<float>((fConst2 * fTemp5), ((fSlow3 * (fConst1 - fTemp5)) + 1.0f))) * (((((((((((((((0.5f * ftbl0mydspSIG0[int((65536.0f * fRec68[0]))]) + (0.25f * ftbl0mydspSIG0[int((65536.0f * fRec69[0]))])) + (0.125f * ftbl0mydspSIG0[int((65536.0f * fRec70[0]))])) + (0.0625f * ftbl0mydspSIG0[int((65536.0f * fRec71[0]))])) + (0.0317000002f * ftbl0mydspSIG0[int((65536.0f * fRec72[0]))])) + (0.0149999997f * ftbl0mydspSIG0[int((65536.0f * fRec73[0]))])) + (0.00749999983f * ftbl0mydspSIG0[int((65536.0f * fRec74[0]))])) + (0.00374999992f * ftbl0mydspSIG0[int((65536.0f * fRec75[0]))])) + (0.00187499996f * ftbl0mydspSIG0[int((65536.0f * fRec76[0]))])) + (0.000937499979f * ftbl0mydspSIG0[int((65536.0f * fRec77[0]))])) + (0.00046874999f * ftbl0mydspSIG0[int((65536.0f * fRec78[0]))])) + (0.000234380001f * ftbl0mydspSIG0[int((65536.0f * fRec79[0]))])) + (0.00011719f * ftbl0mydspSIG0[int((65536.0f * fRec80[0]))])) + (5.85999987e-05f * ftbl0mydspSIG0[int((65536.0f * fRec81[0]))])) + (2.92999994e-05f * ftbl0mydspSIG0[int((65536.0f * fRec82[0]))])))) + (std::max<float>(0.0f, std::min<float>((fConst2 * fTemp6), ((fSlow3 * (fConst1 - fTemp6)) + 1.0f))) * (((((((((((((((0.5f * ftbl0mydspSIG0[int((65536.0f * fRec84[0]))]) + (0.25f * ftbl0mydspSIG0[int((65536.0f * fRec85[0]))])) + (0.125f * ftbl0mydspSIG0[int((65536.0f * fRec86[0]))])) + (0.0625f * ftbl0mydspSIG0[int((65536.0f * fRec87[0]))])) + (0.0317000002f * ftbl0mydspSIG0[int((65536.0f * fRec88[0]))])) + (0.0149999997f * ftbl0mydspSIG0[int((65536.0f * fRec89[0]))])) + (0.00749999983f * ftbl0mydspSIG0[int((65536.0f * fRec90[0]))])) + (0.00374999992f * ftbl0mydspSIG0[int((65536.0f * fRec91[0]))])) + (0.00187499996f * ftbl0mydspSIG0[int((65536.0f * fRec92[0]))])) + (0.000937499979f * ftbl0mydspSIG0[int((65536.0f * fRec93[0]))])) + (0.00046874999f * ftbl0mydspSIG0[int((65536.0f * fRec94[0]))])) + (0.000234380001f * ftbl0mydspSIG0[int((65536.0f * fRec95[0]))])) + (0.00011719f * ftbl0mydspSIG0[int((65536.0f * fRec96[0]))])) + (5.85999987e-05f * ftbl0mydspSIG0[int((65536.0f * fRec97[0]))])) + (2.92999994e-05f * ftbl0mydspSIG0[int((65536.0f * fRec98[0]))])))) + (std::max<float>(0.0f, std::min<float>((fConst2 * fTemp7), ((fSlow3 * (fConst1 - fTemp7)) + 1.0f))) * (((((((((((((((0.5f * ftbl0mydspSIG0[int((65536.0f * fRec100[0]))]) + (0.25f * ftbl0mydspSIG0[int((65536.0f * fRec101[0]))])) + (0.125f * ftbl0mydspSIG0[int((65536.0f * fRec102[0]))])) + (0.0625f * ftbl0mydspSIG0[int((65536.0f * fRec103[0]))])) + (0.0317000002f * ftbl0mydspSIG0[int((65536.0f * fRec104[0]))])) + (0.0149999997f * ftbl0mydspSIG0[int((65536.0f * fRec105[0]))])) + (0.00749999983f * ftbl0mydspSIG0[int((65536.0f * fRec106[0]))])) + (0.00374999992f * ftbl0mydspSIG0[int((65536.0f * fRec107[0]))])) + (0.00187499996f * ftbl0mydspSIG0[int((65536.0f * fRec108[0]))])) + (0.000937499979f * ftbl0mydspSIG0[int((65536.0f * fRec109[0]))])) + (0.00046874999f * ftbl0mydspSIG0[int((65536.0f * fRec110[0]))])) + (0.000234380001f * ftbl0mydspSIG0[int((65536.0f * fRec111[0]))])) + (0.00011719f * ftbl0mydspSIG0[int((65536.0f * fRec112[0]))])) + (5.85999987e-05f * ftbl0mydspSIG0[int((65536.0f * fRec113[0]))])) + (2.92999994e-05f * ftbl0mydspSIG0[int((65536.0f * fRec114[0]))])))))) + 0.100000001f)));
			float fTemp9 = (1.0f - (0.333333343f * mydsp_faustpower2_f(fTemp8)));
			float fTemp10 = ((fTemp0 * fTemp8) * fTemp9);
			float fTemp11 = (1.0f - (0.5f * fTemp0));
			fRec128[0] = (0.0f - (fConst125 * ((fConst126 * fRec128[1]) - (fRec124[1] + fRec124[2]))));
			fRec127[0] = ((fConst121 * fRec127[1]) + (fConst122 * (fRec124[1] + (fConst123 * fRec128[0]))));
			fVec7[(IOTA & 32767)] = ((0.353553385f * fRec127[0]) + 9.99999968e-21f);
			fVec8[(IOTA & 16383)] = (fTemp8 * fTemp9);
			float fTemp12 = (0.300000012f * fVec8[((IOTA - iConst129) & 16383)]);
			float fTemp13 = (((0.600000024f * fRec125[1]) + fVec7[((IOTA - iConst128) & 32767)]) - fTemp12);
			fVec9[(IOTA & 2047)] = fTemp13;
			fRec125[0] = fVec9[((IOTA - iConst130) & 2047)];
			float fRec126 = (0.0f - (0.600000024f * fTemp13));
			fRec132[0] = (0.0f - (fConst125 * ((fConst126 * fRec132[1]) - (fRec120[1] + fRec120[2]))));
			fRec131[0] = ((fConst139 * fRec131[1]) + (fConst140 * (fRec120[1] + (fConst141 * fRec132[0]))));
			fVec10[(IOTA & 32767)] = ((0.353553385f * fRec131[0]) + 9.99999968e-21f);
			float fTemp14 = (((0.600000024f * fRec129[1]) + fVec10[((IOTA - iConst143) & 32767)]) - fTemp12);
			fVec11[(IOTA & 4095)] = fTemp14;
			fRec129[0] = fVec11[((IOTA - iConst144) & 4095)];
			float fRec130 = (0.0f - (0.600000024f * fTemp14));
			fRec136[0] = (0.0f - (fConst125 * ((fConst126 * fRec136[1]) - (fRec122[1] + fRec122[2]))));
			fRec135[0] = ((fConst153 * fRec135[1]) + (fConst154 * (fRec122[1] + (fConst155 * fRec136[0]))));
			fVec12[(IOTA & 16383)] = ((0.353553385f * fRec135[0]) + 9.99999968e-21f);
			float fTemp15 = (fVec12[((IOTA - iConst157) & 16383)] + (fTemp12 + (0.600000024f * fRec133[1])));
			fVec13[(IOTA & 4095)] = fTemp15;
			fRec133[0] = fVec13[((IOTA - iConst158) & 4095)];
			float fRec134 = (0.0f - (0.600000024f * fTemp15));
			fRec140[0] = (0.0f - (fConst125 * ((fConst126 * fRec140[1]) - (fRec118[1] + fRec118[2]))));
			fRec139[0] = ((fConst167 * fRec139[1]) + (fConst168 * (fRec118[1] + (fConst169 * fRec140[0]))));
			fVec14[(IOTA & 32767)] = ((0.353553385f * fRec139[0]) + 9.99999968e-21f);
			float fTemp16 = (fVec14[((IOTA - iConst171) & 32767)] + (fTemp12 + (0.600000024f * fRec137[1])));
			fVec15[(IOTA & 4095)] = fTemp16;
			fRec137[0] = fVec15[((IOTA - iConst172) & 4095)];
			float fRec138 = (0.0f - (0.600000024f * fTemp16));
			fRec144[0] = (0.0f - (fConst125 * ((fConst126 * fRec144[1]) - (fRec123[1] + fRec123[2]))));
			fRec143[0] = ((fConst181 * fRec143[1]) + (fConst182 * (fRec123[1] + (fConst183 * fRec144[0]))));
			fVec16[(IOTA & 16383)] = ((0.353553385f * fRec143[0]) + 9.99999968e-21f);
			float fTemp17 = (fVec16[((IOTA - iConst185) & 16383)] - (fTemp12 + (0.600000024f * fRec141[1])));
			fVec17[(IOTA & 2047)] = fTemp17;
			fRec141[0] = fVec17[((IOTA - iConst186) & 2047)];
			float fRec142 = (0.600000024f * fTemp17);
			fRec148[0] = (0.0f - (fConst125 * ((fConst126 * fRec148[1]) - (fRec119[1] + fRec119[2]))));
			fRec147[0] = ((fConst195 * fRec147[1]) + (fConst196 * (fRec119[1] + (fConst197 * fRec148[0]))));
			fVec18[(IOTA & 16383)] = ((0.353553385f * fRec147[0]) + 9.99999968e-21f);
			float fTemp18 = (fVec18[((IOTA - iConst199) & 16383)] - (fTemp12 + (0.600000024f * fRec145[1])));
			fVec19[(IOTA & 4095)] = fTemp18;
			fRec145[0] = fVec19[((IOTA - iConst200) & 4095)];
			float fRec146 = (0.600000024f * fTemp18);
			fRec152[0] = (0.0f - (fConst125 * ((fConst126 * fRec152[1]) - (fRec121[1] + fRec121[2]))));
			fRec151[0] = ((fConst209 * fRec151[1]) + (fConst210 * (fRec121[1] + (fConst211 * fRec152[0]))));
			fVec20[(IOTA & 16383)] = ((0.353553385f * fRec151[0]) + 9.99999968e-21f);
			float fTemp19 = ((fTemp12 + fVec20[((IOTA - iConst213) & 16383)]) - (0.600000024f * fRec149[1]));
			fVec21[(IOTA & 4095)] = fTemp19;
			fRec149[0] = fVec21[((IOTA - iConst214) & 4095)];
			float fRec150 = (0.600000024f * fTemp19);
			fRec156[0] = (0.0f - (fConst125 * ((fConst126 * fRec156[1]) - (fRec117[1] + fRec117[2]))));
			fRec155[0] = ((fConst223 * fRec155[1]) + (fConst224 * (fRec117[1] + (fConst225 * fRec156[0]))));
			fVec22[(IOTA & 16383)] = ((0.353553385f * fRec155[0]) + 9.99999968e-21f);
			float fTemp20 = ((fVec22[((IOTA - iConst227) & 16383)] + fTemp12) - (0.600000024f * fRec153[1]));
			fVec23[(IOTA & 2047)] = fTemp20;
			fRec153[0] = fVec23[((IOTA - iConst228) & 2047)];
			float fRec154 = (0.600000024f * fTemp20);
			float fTemp21 = (fRec154 + fRec150);
			float fTemp22 = (fRec142 + (fRec146 + fTemp21));
			fRec117[0] = (fRec125[1] + (fRec129[1] + (fRec133[1] + (fRec137[1] + (fRec141[1] + (fRec145[1] + (fRec149[1] + (fRec153[1] + (fRec126 + (fRec130 + (fRec134 + (fRec138 + fTemp22))))))))))));
			fRec118[0] = ((fRec141[1] + (fRec145[1] + (fRec149[1] + (fRec153[1] + fTemp22)))) - (fRec125[1] + (fRec129[1] + (fRec133[1] + (fRec137[1] + (fRec126 + (fRec130 + (fRec138 + fRec134))))))));
			float fTemp23 = (fRec146 + fRec142);
			fRec119[0] = ((fRec133[1] + (fRec137[1] + (fRec149[1] + (fRec153[1] + (fRec134 + (fRec138 + fTemp21)))))) - (fRec125[1] + (fRec129[1] + (fRec141[1] + (fRec145[1] + (fRec126 + (fRec130 + fTemp23)))))));
			fRec120[0] = ((fRec125[1] + (fRec129[1] + (fRec149[1] + (fRec153[1] + (fRec126 + (fRec130 + fTemp21)))))) - (fRec133[1] + (fRec137[1] + (fRec141[1] + (fRec145[1] + (fRec134 + (fRec138 + fTemp23)))))));
			float fTemp24 = (fRec154 + fRec146);
			float fTemp25 = (fRec150 + fRec142);
			fRec121[0] = ((fRec129[1] + (fRec137[1] + (fRec145[1] + (fRec153[1] + (fRec130 + (fRec138 + fTemp24)))))) - (fRec125[1] + (fRec133[1] + (fRec141[1] + (fRec149[1] + (fRec126 + (fRec134 + fTemp25)))))));
			fRec122[0] = ((fRec125[1] + (fRec133[1] + (fRec145[1] + (fRec153[1] + (fRec126 + (fRec134 + fTemp24)))))) - (fRec129[1] + (fRec137[1] + (fRec141[1] + (fRec149[1] + (fRec130 + (fRec138 + fTemp25)))))));
			float fTemp26 = (fRec154 + fRec142);
			float fTemp27 = (fRec150 + fRec146);
			fRec123[0] = ((fRec125[1] + (fRec137[1] + (fRec141[1] + (fRec153[1] + (fRec126 + (fRec138 + fTemp26)))))) - (fRec129[1] + (fRec133[1] + (fRec145[1] + (fRec149[1] + (fRec130 + (fRec134 + fTemp27)))))));
			fRec124[0] = ((fRec129[1] + (fRec133[1] + (fRec141[1] + (fRec153[1] + (fRec130 + (fRec134 + fTemp26)))))) - (fRec125[1] + (fRec137[1] + (fRec145[1] + (fRec149[1] + (fRec126 + (fRec138 + fTemp27)))))));
			float fTemp28 = (0.370000005f * (fRec118[0] + fRec119[0]));
			float fTemp29 = (fConst229 * fRec116[1]);
			fRec116[0] = (fTemp28 - (fTemp29 + (fConst111 * fRec116[2])));
			float fTemp30 = (fConst111 * fRec116[0]);
			float fTemp31 = (0.5f * ((fTemp30 + (fRec116[2] + (fTemp28 + fTemp29))) + ((fTemp30 + (fTemp29 + fRec116[2])) - fTemp28)));
			float fTemp32 = (fConst230 * fRec115[1]);
			fRec115[0] = (fTemp31 - (fTemp32 + (fConst109 * fRec115[2])));
			float fTemp33 = (fConst109 * fRec115[0]);
			output0[i] = FAUSTFLOAT((0.5f * (fRec0[0] * (fTemp10 + (fTemp11 * ((fTemp33 + (fRec115[2] + (fTemp31 + fTemp32))) + ((fTemp33 + (fTemp32 + fRec115[2])) - fTemp31)))))));
			float fTemp34 = (0.370000005f * (fRec118[0] - fRec119[0]));
			float fTemp35 = (fConst229 * fRec158[1]);
			fRec158[0] = (fTemp34 - (fTemp35 + (fConst111 * fRec158[2])));
			float fTemp36 = (fConst111 * fRec158[0]);
			float fTemp37 = (0.5f * ((fTemp36 + (fRec158[2] + (fTemp34 + fTemp35))) + ((fTemp36 + (fTemp35 + fRec158[2])) - fTemp34)));
			float fTemp38 = (fConst230 * fRec157[1]);
			fRec157[0] = (fTemp37 - (fTemp38 + (fConst109 * fRec157[2])));
			float fTemp39 = (fConst109 * fRec157[0]);
			output1[i] = FAUSTFLOAT((0.5f * (fRec0[0] * (fTemp10 + (fTemp11 * ((fTemp39 + (fRec157[2] + (fTemp37 + fTemp38))) + ((fTemp39 + (fTemp38 + fRec157[2])) - fTemp37)))))));
			fRec0[1] = fRec0[0];
			fRec1[1] = fRec1[0];
			fVec0[1] = fVec0[0];
			iRec2[1] = iRec2[0];
			fRec4[1] = fRec4[0];
			fRec5[1] = fRec5[0];
			fRec6[1] = fRec6[0];
			fRec7[1] = fRec7[0];
			fRec8[1] = fRec8[0];
			fRec9[1] = fRec9[0];
			fRec10[1] = fRec10[0];
			fRec11[1] = fRec11[0];
			fRec12[1] = fRec12[0];
			fRec13[1] = fRec13[0];
			fRec14[1] = fRec14[0];
			fRec15[1] = fRec15[0];
			fRec16[1] = fRec16[0];
			fRec17[1] = fRec17[0];
			fRec18[1] = fRec18[0];
			fVec1[1] = fVec1[0];
			iRec19[1] = iRec19[0];
			fRec20[1] = fRec20[0];
			fRec21[1] = fRec21[0];
			fRec22[1] = fRec22[0];
			fRec23[1] = fRec23[0];
			fRec24[1] = fRec24[0];
			fRec25[1] = fRec25[0];
			fRec26[1] = fRec26[0];
			fRec27[1] = fRec27[0];
			fRec28[1] = fRec28[0];
			fRec29[1] = fRec29[0];
			fRec30[1] = fRec30[0];
			fRec31[1] = fRec31[0];
			fRec32[1] = fRec32[0];
			fRec33[1] = fRec33[0];
			fRec34[1] = fRec34[0];
			fVec2[1] = fVec2[0];
			iRec35[1] = iRec35[0];
			fRec36[1] = fRec36[0];
			fRec37[1] = fRec37[0];
			fRec38[1] = fRec38[0];
			fRec39[1] = fRec39[0];
			fRec40[1] = fRec40[0];
			fRec41[1] = fRec41[0];
			fRec42[1] = fRec42[0];
			fRec43[1] = fRec43[0];
			fRec44[1] = fRec44[0];
			fRec45[1] = fRec45[0];
			fRec46[1] = fRec46[0];
			fRec47[1] = fRec47[0];
			fRec48[1] = fRec48[0];
			fRec49[1] = fRec49[0];
			fRec50[1] = fRec50[0];
			fVec3[1] = fVec3[0];
			iRec51[1] = iRec51[0];
			fRec52[1] = fRec52[0];
			fRec53[1] = fRec53[0];
			fRec54[1] = fRec54[0];
			fRec55[1] = fRec55[0];
			fRec56[1] = fRec56[0];
			fRec57[1] = fRec57[0];
			fRec58[1] = fRec58[0];
			fRec59[1] = fRec59[0];
			fRec60[1] = fRec60[0];
			fRec61[1] = fRec61[0];
			fRec62[1] = fRec62[0];
			fRec63[1] = fRec63[0];
			fRec64[1] = fRec64[0];
			fRec65[1] = fRec65[0];
			fRec66[1] = fRec66[0];
			fVec4[1] = fVec4[0];
			iRec67[1] = iRec67[0];
			fRec68[1] = fRec68[0];
			fRec69[1] = fRec69[0];
			fRec70[1] = fRec70[0];
			fRec71[1] = fRec71[0];
			fRec72[1] = fRec72[0];
			fRec73[1] = fRec73[0];
			fRec74[1] = fRec74[0];
			fRec75[1] = fRec75[0];
			fRec76[1] = fRec76[0];
			fRec77[1] = fRec77[0];
			fRec78[1] = fRec78[0];
			fRec79[1] = fRec79[0];
			fRec80[1] = fRec80[0];
			fRec81[1] = fRec81[0];
			fRec82[1] = fRec82[0];
			fVec5[1] = fVec5[0];
			iRec83[1] = iRec83[0];
			fRec84[1] = fRec84[0];
			fRec85[1] = fRec85[0];
			fRec86[1] = fRec86[0];
			fRec87[1] = fRec87[0];
			fRec88[1] = fRec88[0];
			fRec89[1] = fRec89[0];
			fRec90[1] = fRec90[0];
			fRec91[1] = fRec91[0];
			fRec92[1] = fRec92[0];
			fRec93[1] = fRec93[0];
			fRec94[1] = fRec94[0];
			fRec95[1] = fRec95[0];
			fRec96[1] = fRec96[0];
			fRec97[1] = fRec97[0];
			fRec98[1] = fRec98[0];
			fVec6[1] = fVec6[0];
			iRec99[1] = iRec99[0];
			fRec100[1] = fRec100[0];
			fRec101[1] = fRec101[0];
			fRec102[1] = fRec102[0];
			fRec103[1] = fRec103[0];
			fRec104[1] = fRec104[0];
			fRec105[1] = fRec105[0];
			fRec106[1] = fRec106[0];
			fRec107[1] = fRec107[0];
			fRec108[1] = fRec108[0];
			fRec109[1] = fRec109[0];
			fRec110[1] = fRec110[0];
			fRec111[1] = fRec111[0];
			fRec112[1] = fRec112[0];
			fRec113[1] = fRec113[0];
			fRec114[1] = fRec114[0];
			fRec128[1] = fRec128[0];
			fRec127[1] = fRec127[0];
			IOTA = (IOTA + 1);
			fRec125[1] = fRec125[0];
			fRec132[1] = fRec132[0];
			fRec131[1] = fRec131[0];
			fRec129[1] = fRec129[0];
			fRec136[1] = fRec136[0];
			fRec135[1] = fRec135[0];
			fRec133[1] = fRec133[0];
			fRec140[1] = fRec140[0];
			fRec139[1] = fRec139[0];
			fRec137[1] = fRec137[0];
			fRec144[1] = fRec144[0];
			fRec143[1] = fRec143[0];
			fRec141[1] = fRec141[0];
			fRec148[1] = fRec148[0];
			fRec147[1] = fRec147[0];
			fRec145[1] = fRec145[0];
			fRec152[1] = fRec152[0];
			fRec151[1] = fRec151[0];
			fRec149[1] = fRec149[0];
			fRec156[1] = fRec156[0];
			fRec155[1] = fRec155[0];
			fRec153[1] = fRec153[0];
			fRec117[2] = fRec117[1];
			fRec117[1] = fRec117[0];
			fRec118[2] = fRec118[1];
			fRec118[1] = fRec118[0];
			fRec119[2] = fRec119[1];
			fRec119[1] = fRec119[0];
			fRec120[2] = fRec120[1];
			fRec120[1] = fRec120[0];
			fRec121[2] = fRec121[1];
			fRec121[1] = fRec121[0];
			fRec122[2] = fRec122[1];
			fRec122[1] = fRec122[0];
			fRec123[2] = fRec123[1];
			fRec123[1] = fRec123[0];
			fRec124[2] = fRec124[1];
			fRec124[1] = fRec124[0];
			fRec116[2] = fRec116[1];
			fRec116[1] = fRec116[0];
			fRec115[2] = fRec115[1];
			fRec115[1] = fRec115[0];
			fRec158[2] = fRec158[1];
			fRec158[1] = fRec158[0];
			fRec157[2] = fRec157[1];
			fRec157[1] = fRec157[0];
		}
	}

};

#endif
