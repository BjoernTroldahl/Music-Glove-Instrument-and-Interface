/* ------------------------------------------------------------
name: "lyd"
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
	
	int iRec2[2];
	
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
		for (int l3 = 0; (l3 < 2); l3 = (l3 + 1)) {
			iRec2[l3] = 0;
		}
	}
	
	void fillmydspSIG0(int count, float* table) {
		for (int i = 0; (i < count); i = (i + 1)) {
			iRec2[0] = (iRec2[1] + 1);
			table[i] = std::sin((9.58738019e-05f * float((iRec2[0] + -1))));
			iRec2[1] = iRec2[0];
		}
	}

};

static mydspSIG0* newmydspSIG0() { return (mydspSIG0*)new mydspSIG0(); }
static void deletemydspSIG0(mydspSIG0* dsp) { delete dsp; }

static float ftbl0mydspSIG0[65536];

#ifndef FAUSTCLASS 
#define FAUSTCLASS mydsp
#endif

#ifdef __APPLE__ 
#define exp10f __exp10f
#define exp10 __exp10
#endif

class mydsp : public dsp {
	
 private:
	
	int fSampleRate;
	float fConst0;
	float fConst1;
	float fConst2;
	FAUSTFLOAT fButton0;
	float fVec0[2];
	float fRec0[2];
	float fConst3;
	FAUSTFLOAT fHslider0;
	int iRec1[2];
	float fConst4;
	float fRec3[2];
	float fConst5;
	float fRec4[2];
	float fConst6;
	float fRec5[2];
	FAUSTFLOAT fButton1;
	float fVec1[2];
	float fRec6[2];
	int iRec7[2];
	float fConst7;
	float fRec8[2];
	float fConst8;
	float fRec9[2];
	float fConst9;
	float fRec10[2];
	FAUSTFLOAT fButton2;
	float fVec2[2];
	float fRec11[2];
	int iRec12[2];
	float fConst10;
	float fRec13[2];
	float fConst11;
	float fRec14[2];
	float fConst12;
	float fRec15[2];
	FAUSTFLOAT fButton3;
	float fVec3[2];
	float fRec16[2];
	int iRec17[2];
	float fConst13;
	float fRec18[2];
	float fConst14;
	float fRec19[2];
	float fConst15;
	float fRec20[2];
	FAUSTFLOAT fButton4;
	float fVec4[2];
	float fRec21[2];
	int iRec22[2];
	float fConst16;
	float fRec23[2];
	float fConst17;
	float fRec24[2];
	float fConst18;
	float fRec25[2];
	FAUSTFLOAT fButton5;
	float fVec5[2];
	float fRec26[2];
	int iRec27[2];
	float fConst19;
	float fRec28[2];
	float fConst20;
	float fRec29[2];
	float fConst21;
	float fRec30[2];
	FAUSTFLOAT fButton6;
	float fVec6[2];
	float fRec31[2];
	int iRec32[2];
	float fConst22;
	float fRec33[2];
	float fConst23;
	float fRec34[2];
	float fConst24;
	float fRec35[2];
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "0.1");
		m->declare("compile_options", "-lang cpp -es 1 -scal -ftz 0");
		m->declare("envelopes.lib/adsr:author", "Yann Orlarey");
		m->declare("envelopes.lib/author", "GRAME");
		m->declare("envelopes.lib/copyright", "GRAME");
		m->declare("envelopes.lib/license", "LGPL with exception");
		m->declare("envelopes.lib/name", "Faust Envelope Library");
		m->declare("envelopes.lib/version", "0.1");
		m->declare("filename", "lyd.dsp");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.3");
		m->declare("name", "lyd");
		m->declare("oscillators.lib/name", "Faust Oscillator Library");
		m->declare("oscillators.lib/version", "0.1");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "0.1");
	}

	virtual int getNumInputs() {
		return 0;
	}
	virtual int getNumOutputs() {
		return 1;
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
		fConst3 = (0.0f - (1.0f / std::max<float>(1.0f, (2.0f * fConst0))));
		fConst4 = (220.0f / fConst0);
		fConst5 = (440.0f / fConst0);
		fConst6 = (660.0f / fConst0);
		fConst7 = (246.940002f / fConst0);
		fConst8 = (493.880005f / fConst0);
		fConst9 = (740.820007f / fConst0);
		fConst10 = (261.630005f / fConst0);
		fConst11 = (523.26001f / fConst0);
		fConst12 = (784.890015f / fConst0);
		fConst13 = (293.660004f / fConst0);
		fConst14 = (587.320007f / fConst0);
		fConst15 = (880.97998f / fConst0);
		fConst16 = (329.630005f / fConst0);
		fConst17 = (659.26001f / fConst0);
		fConst18 = (988.890015f / fConst0);
		fConst19 = (349.230011f / fConst0);
		fConst20 = (698.460022f / fConst0);
		fConst21 = (1047.68994f / fConst0);
		fConst22 = (392.0f / fConst0);
		fConst23 = (784.0f / fConst0);
		fConst24 = (1176.0f / fConst0);
	}
	
	virtual void instanceResetUserInterface() {
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
			fVec0[l0] = 0.0f;
		}
		for (int l1 = 0; (l1 < 2); l1 = (l1 + 1)) {
			fRec0[l1] = 0.0f;
		}
		for (int l2 = 0; (l2 < 2); l2 = (l2 + 1)) {
			iRec1[l2] = 0;
		}
		for (int l4 = 0; (l4 < 2); l4 = (l4 + 1)) {
			fRec3[l4] = 0.0f;
		}
		for (int l5 = 0; (l5 < 2); l5 = (l5 + 1)) {
			fRec4[l5] = 0.0f;
		}
		for (int l6 = 0; (l6 < 2); l6 = (l6 + 1)) {
			fRec5[l6] = 0.0f;
		}
		for (int l7 = 0; (l7 < 2); l7 = (l7 + 1)) {
			fVec1[l7] = 0.0f;
		}
		for (int l8 = 0; (l8 < 2); l8 = (l8 + 1)) {
			fRec6[l8] = 0.0f;
		}
		for (int l9 = 0; (l9 < 2); l9 = (l9 + 1)) {
			iRec7[l9] = 0;
		}
		for (int l10 = 0; (l10 < 2); l10 = (l10 + 1)) {
			fRec8[l10] = 0.0f;
		}
		for (int l11 = 0; (l11 < 2); l11 = (l11 + 1)) {
			fRec9[l11] = 0.0f;
		}
		for (int l12 = 0; (l12 < 2); l12 = (l12 + 1)) {
			fRec10[l12] = 0.0f;
		}
		for (int l13 = 0; (l13 < 2); l13 = (l13 + 1)) {
			fVec2[l13] = 0.0f;
		}
		for (int l14 = 0; (l14 < 2); l14 = (l14 + 1)) {
			fRec11[l14] = 0.0f;
		}
		for (int l15 = 0; (l15 < 2); l15 = (l15 + 1)) {
			iRec12[l15] = 0;
		}
		for (int l16 = 0; (l16 < 2); l16 = (l16 + 1)) {
			fRec13[l16] = 0.0f;
		}
		for (int l17 = 0; (l17 < 2); l17 = (l17 + 1)) {
			fRec14[l17] = 0.0f;
		}
		for (int l18 = 0; (l18 < 2); l18 = (l18 + 1)) {
			fRec15[l18] = 0.0f;
		}
		for (int l19 = 0; (l19 < 2); l19 = (l19 + 1)) {
			fVec3[l19] = 0.0f;
		}
		for (int l20 = 0; (l20 < 2); l20 = (l20 + 1)) {
			fRec16[l20] = 0.0f;
		}
		for (int l21 = 0; (l21 < 2); l21 = (l21 + 1)) {
			iRec17[l21] = 0;
		}
		for (int l22 = 0; (l22 < 2); l22 = (l22 + 1)) {
			fRec18[l22] = 0.0f;
		}
		for (int l23 = 0; (l23 < 2); l23 = (l23 + 1)) {
			fRec19[l23] = 0.0f;
		}
		for (int l24 = 0; (l24 < 2); l24 = (l24 + 1)) {
			fRec20[l24] = 0.0f;
		}
		for (int l25 = 0; (l25 < 2); l25 = (l25 + 1)) {
			fVec4[l25] = 0.0f;
		}
		for (int l26 = 0; (l26 < 2); l26 = (l26 + 1)) {
			fRec21[l26] = 0.0f;
		}
		for (int l27 = 0; (l27 < 2); l27 = (l27 + 1)) {
			iRec22[l27] = 0;
		}
		for (int l28 = 0; (l28 < 2); l28 = (l28 + 1)) {
			fRec23[l28] = 0.0f;
		}
		for (int l29 = 0; (l29 < 2); l29 = (l29 + 1)) {
			fRec24[l29] = 0.0f;
		}
		for (int l30 = 0; (l30 < 2); l30 = (l30 + 1)) {
			fRec25[l30] = 0.0f;
		}
		for (int l31 = 0; (l31 < 2); l31 = (l31 + 1)) {
			fVec5[l31] = 0.0f;
		}
		for (int l32 = 0; (l32 < 2); l32 = (l32 + 1)) {
			fRec26[l32] = 0.0f;
		}
		for (int l33 = 0; (l33 < 2); l33 = (l33 + 1)) {
			iRec27[l33] = 0;
		}
		for (int l34 = 0; (l34 < 2); l34 = (l34 + 1)) {
			fRec28[l34] = 0.0f;
		}
		for (int l35 = 0; (l35 < 2); l35 = (l35 + 1)) {
			fRec29[l35] = 0.0f;
		}
		for (int l36 = 0; (l36 < 2); l36 = (l36 + 1)) {
			fRec30[l36] = 0.0f;
		}
		for (int l37 = 0; (l37 < 2); l37 = (l37 + 1)) {
			fVec6[l37] = 0.0f;
		}
		for (int l38 = 0; (l38 < 2); l38 = (l38 + 1)) {
			fRec31[l38] = 0.0f;
		}
		for (int l39 = 0; (l39 < 2); l39 = (l39 + 1)) {
			iRec32[l39] = 0;
		}
		for (int l40 = 0; (l40 < 2); l40 = (l40 + 1)) {
			fRec33[l40] = 0.0f;
		}
		for (int l41 = 0; (l41 < 2); l41 = (l41 + 1)) {
			fRec34[l41] = 0.0f;
		}
		for (int l42 = 0; (l42 < 2); l42 = (l42 + 1)) {
			fRec35[l42] = 0.0f;
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
		ui_interface->openVerticalBox("lyd");
		ui_interface->addButton("A", &fButton0);
		ui_interface->addButton("B", &fButton1);
		ui_interface->addButton("C", &fButton2);
		ui_interface->addButton("D", &fButton3);
		ui_interface->addButton("E", &fButton4);
		ui_interface->addButton("F", &fButton5);
		ui_interface->addButton("G", &fButton6);
		ui_interface->addHorizontalSlider("rel", &fHslider0, 1.0f, 1.0f, 10.0f, 0.00999999978f);
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) {
		FAUSTFLOAT* output0 = outputs[0];
		float fSlow0 = float(fButton0);
		float fSlow1 = (2.0f / std::max<float>(1.0f, (fConst0 * float(fHslider0))));
		int iSlow2 = (fSlow0 == 0.0f);
		float fSlow3 = float(fButton1);
		int iSlow4 = (fSlow3 == 0.0f);
		float fSlow5 = float(fButton2);
		int iSlow6 = (fSlow5 == 0.0f);
		float fSlow7 = float(fButton3);
		int iSlow8 = (fSlow7 == 0.0f);
		float fSlow9 = float(fButton4);
		int iSlow10 = (fSlow9 == 0.0f);
		float fSlow11 = float(fButton5);
		int iSlow12 = (fSlow11 == 0.0f);
		float fSlow13 = float(fButton6);
		int iSlow14 = (fSlow13 == 0.0f);
		for (int i = 0; (i < count); i = (i + 1)) {
			fVec0[0] = fSlow0;
			fRec0[0] = (fSlow0 + (fRec0[1] * float((fVec0[1] >= fSlow0))));
			iRec1[0] = (iSlow2 * (iRec1[1] + 1));
			fRec3[0] = (fConst4 + (fRec3[1] - std::floor((fConst4 + fRec3[1]))));
			fRec4[0] = (fConst5 + (fRec4[1] - std::floor((fConst5 + fRec4[1]))));
			fRec5[0] = (fConst6 + (fRec5[1] - std::floor((fConst6 + fRec5[1]))));
			fVec1[0] = fSlow3;
			fRec6[0] = (fSlow3 + (fRec6[1] * float((fVec1[1] >= fSlow3))));
			iRec7[0] = (iSlow4 * (iRec7[1] + 1));
			fRec8[0] = (fConst7 + (fRec8[1] - std::floor((fConst7 + fRec8[1]))));
			fRec9[0] = (fConst8 + (fRec9[1] - std::floor((fConst8 + fRec9[1]))));
			fRec10[0] = (fConst9 + (fRec10[1] - std::floor((fConst9 + fRec10[1]))));
			fVec2[0] = fSlow5;
			fRec11[0] = (fSlow5 + (fRec11[1] * float((fVec2[1] >= fSlow5))));
			iRec12[0] = (iSlow6 * (iRec12[1] + 1));
			fRec13[0] = (fConst10 + (fRec13[1] - std::floor((fConst10 + fRec13[1]))));
			fRec14[0] = (fConst11 + (fRec14[1] - std::floor((fConst11 + fRec14[1]))));
			fRec15[0] = (fConst12 + (fRec15[1] - std::floor((fConst12 + fRec15[1]))));
			fVec3[0] = fSlow7;
			fRec16[0] = (fSlow7 + (fRec16[1] * float((fVec3[1] >= fSlow7))));
			iRec17[0] = (iSlow8 * (iRec17[1] + 1));
			fRec18[0] = (fConst13 + (fRec18[1] - std::floor((fConst13 + fRec18[1]))));
			fRec19[0] = (fConst14 + (fRec19[1] - std::floor((fConst14 + fRec19[1]))));
			fRec20[0] = (fConst15 + (fRec20[1] - std::floor((fConst15 + fRec20[1]))));
			fVec4[0] = fSlow9;
			fRec21[0] = (fSlow9 + (fRec21[1] * float((fVec4[1] >= fSlow9))));
			iRec22[0] = (iSlow10 * (iRec22[1] + 1));
			fRec23[0] = (fConst16 + (fRec23[1] - std::floor((fConst16 + fRec23[1]))));
			fRec24[0] = (fConst17 + (fRec24[1] - std::floor((fConst17 + fRec24[1]))));
			fRec25[0] = (fConst18 + (fRec25[1] - std::floor((fConst18 + fRec25[1]))));
			fVec5[0] = fSlow11;
			fRec26[0] = (fSlow11 + (fRec26[1] * float((fVec5[1] >= fSlow11))));
			iRec27[0] = (iSlow12 * (iRec27[1] + 1));
			fRec28[0] = (fConst19 + (fRec28[1] - std::floor((fConst19 + fRec28[1]))));
			fRec29[0] = (fConst20 + (fRec29[1] - std::floor((fConst20 + fRec29[1]))));
			fRec30[0] = (fConst21 + (fRec30[1] - std::floor((fConst21 + fRec30[1]))));
			fVec6[0] = fSlow13;
			fRec31[0] = (fSlow13 + (fRec31[1] * float((fVec6[1] >= fSlow13))));
			iRec32[0] = (iSlow14 * (iRec32[1] + 1));
			fRec33[0] = (fConst22 + (fRec33[1] - std::floor((fConst22 + fRec33[1]))));
			fRec34[0] = (fConst23 + (fRec34[1] - std::floor((fConst23 + fRec34[1]))));
			fRec35[0] = (fConst24 + (fRec35[1] - std::floor((fConst24 + fRec35[1]))));
			output0[i] = FAUSTFLOAT((0.333333343f * ((std::max<float>(0.0f, (std::min<float>((fConst2 * fRec0[0]), std::max<float>(((fConst3 * (fConst1 - fRec0[0])) + 1.0f), 2.0f)) - (fSlow1 * float(iRec1[0])))) * (((0.5f * ftbl0mydspSIG0[int((65536.0f * fRec3[0]))]) + (0.25f * ftbl0mydspSIG0[int((65536.0f * fRec4[0]))])) + (0.125f * ftbl0mydspSIG0[int((65536.0f * fRec5[0]))]))) + ((((((std::max<float>(0.0f, (std::min<float>((fConst2 * fRec6[0]), std::max<float>(((fConst3 * (fConst1 - fRec6[0])) + 1.0f), 2.0f)) - (fSlow1 * float(iRec7[0])))) * (((0.5f * ftbl0mydspSIG0[int((65536.0f * fRec8[0]))]) + (0.25f * ftbl0mydspSIG0[int((65536.0f * fRec9[0]))])) + (0.125f * ftbl0mydspSIG0[int((65536.0f * fRec10[0]))]))) + (std::max<float>(0.0f, (std::min<float>((fConst2 * fRec11[0]), std::max<float>(((fConst3 * (fConst1 - fRec11[0])) + 1.0f), 2.0f)) - (fSlow1 * float(iRec12[0])))) * (((0.5f * ftbl0mydspSIG0[int((65536.0f * fRec13[0]))]) + (0.25f * ftbl0mydspSIG0[int((65536.0f * fRec14[0]))])) + (0.125f * ftbl0mydspSIG0[int((65536.0f * fRec15[0]))])))) + (std::max<float>(0.0f, (std::min<float>((fConst2 * fRec16[0]), std::max<float>(((fConst3 * (fConst1 - fRec16[0])) + 1.0f), 2.0f)) - (fSlow1 * float(iRec17[0])))) * (((0.5f * ftbl0mydspSIG0[int((65536.0f * fRec18[0]))]) + (0.25f * ftbl0mydspSIG0[int((65536.0f * fRec19[0]))])) + (0.125f * ftbl0mydspSIG0[int((65536.0f * fRec20[0]))])))) + (std::max<float>(0.0f, (std::min<float>((fConst2 * fRec21[0]), std::max<float>(((fConst3 * (fConst1 - fRec21[0])) + 1.0f), 2.0f)) - (fSlow1 * float(iRec22[0])))) * (((0.5f * ftbl0mydspSIG0[int((65536.0f * fRec23[0]))]) + (0.25f * ftbl0mydspSIG0[int((65536.0f * fRec24[0]))])) + (0.125f * ftbl0mydspSIG0[int((65536.0f * fRec25[0]))])))) + (std::max<float>(0.0f, (std::min<float>((fConst2 * fRec26[0]), std::max<float>(((fConst3 * (fConst1 - fRec26[0])) + 1.0f), 2.0f)) - (fSlow1 * float(iRec27[0])))) * (((0.5f * ftbl0mydspSIG0[int((65536.0f * fRec28[0]))]) + (0.25f * ftbl0mydspSIG0[int((65536.0f * fRec29[0]))])) + (0.125f * ftbl0mydspSIG0[int((65536.0f * fRec30[0]))])))) + (std::max<float>(0.0f, (std::min<float>((fConst2 * fRec31[0]), std::max<float>(((fConst3 * (fConst1 - fRec31[0])) + 1.0f), 2.0f)) - (fSlow1 * float(iRec32[0])))) * (((0.5f * ftbl0mydspSIG0[int((65536.0f * fRec33[0]))]) + (0.25f * ftbl0mydspSIG0[int((65536.0f * fRec34[0]))])) + (0.125f * ftbl0mydspSIG0[int((65536.0f * fRec35[0]))])))))));
			fVec0[1] = fVec0[0];
			fRec0[1] = fRec0[0];
			iRec1[1] = iRec1[0];
			fRec3[1] = fRec3[0];
			fRec4[1] = fRec4[0];
			fRec5[1] = fRec5[0];
			fVec1[1] = fVec1[0];
			fRec6[1] = fRec6[0];
			iRec7[1] = iRec7[0];
			fRec8[1] = fRec8[0];
			fRec9[1] = fRec9[0];
			fRec10[1] = fRec10[0];
			fVec2[1] = fVec2[0];
			fRec11[1] = fRec11[0];
			iRec12[1] = iRec12[0];
			fRec13[1] = fRec13[0];
			fRec14[1] = fRec14[0];
			fRec15[1] = fRec15[0];
			fVec3[1] = fVec3[0];
			fRec16[1] = fRec16[0];
			iRec17[1] = iRec17[0];
			fRec18[1] = fRec18[0];
			fRec19[1] = fRec19[0];
			fRec20[1] = fRec20[0];
			fVec4[1] = fVec4[0];
			fRec21[1] = fRec21[0];
			iRec22[1] = iRec22[0];
			fRec23[1] = fRec23[0];
			fRec24[1] = fRec24[0];
			fRec25[1] = fRec25[0];
			fVec5[1] = fVec5[0];
			fRec26[1] = fRec26[0];
			iRec27[1] = iRec27[0];
			fRec28[1] = fRec28[0];
			fRec29[1] = fRec29[0];
			fRec30[1] = fRec30[0];
			fVec6[1] = fVec6[0];
			fRec31[1] = fRec31[0];
			iRec32[1] = iRec32[0];
			fRec33[1] = fRec33[0];
			fRec34[1] = fRec34[0];
			fRec35[1] = fRec35[0];
		}
	}

};

#endif
