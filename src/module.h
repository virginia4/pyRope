#ifndef MODULE_H
#define MODULE_H

#include <Python.h>
#include <vector>
#include <vagabond/core/Atom.h>
#include <vagabond/core/AtomPosMap.h>


struct AtomObject {
    PyObject_HEAD
    Atom* atom;
} AtomObject;

class AtomWrapper {
public:
    AtomWrapper(PyObject* capsule) : capsule_(capsule), atom_(nullptr) {
        initAtom();
    }

    ~AtomWrapper() {
        // Clean up resources if needed
    }

    bool isValid() const {
        return atom_ != nullptr;
    }

    Atom* getAtom() const {
        return atom_;
    }

private:
    void initAtom() {
        void* atomVal = PyCapsule_GetPointer(capsule_, nullptr);
        atom_ = static_cast<Atom*>(atomVal);
        if (!atomVal) {
            PyErr_SetString(PyExc_TypeError, "Invalid capsule");
        }
    }

    PyObject* capsule_;
    Atom* atom_;
};

// Function declarations
extern "C" {
    PyMODINIT_FUNC PyInit_module(void);
    PyMODINIT_FUNC helen_module_py_init(PyObject *module);
    PyObject* greet(PyObject* self, PyObject * args);
    PyObject* getOneAtomPosition(PyObject* self, PyObject* args);
    PyObject* createAtomObject(PyObject* self, PyObject* args);
    PyObject* getConnectedAtom(PyObject* self, PyObject* args);
    PyObject* setDerivedPosition(PyObject* self, PyObject* args);
    PyObject* getIsConnectedAtom(PyObject* self, PyObject* args);
    PyObject* getAtomName(PyObject* self, PyObject* args);
    PyObject* getAtomNum(PyObject* self, PyObject* args);
    PyObject* getDesc(PyObject* self, PyObject* args);
    PyObject* getBondLengthCount(PyObject* self, PyObject* args);
}

#endif  // MODULE_H
