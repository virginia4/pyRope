#ifndef MODULE_H
#define MODULE_H

#include <Python.h>
#include <vector>
#include <vagabond/core/Environment.h>



// Function declarations
extern "C" {
    PyMODINIT_FUNC PyInit_moduleEnv(void);
    PyMODINIT_FUNC env_module_py_init(PyObject *module);
    static PyObject* getModelManager(PyObject* self, PyObject* args);
    static ModelManager* getModelManagerFromCapsule(PyObject* capsule);
    static PyObject* loadEnvironment(PyObject* self, PyObject* args);
}

#endif  // MODULE_H
