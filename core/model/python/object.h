#ifndef PYTHON_OBJECT_H
#define PYTHON_OBJECT_H

#pragma push_macro("slots")
#undef slots
#include <Python.h>
#pragma pop_macro("slots")

/*! @class Object
 *  @brief Helps manage memory when using Python objects.  */
class Object {
public:
  Object(PyObject *_object) : object(_object) {}
  ~Object() { if (object) Py_CLEAR(object); }
  
  operator PyObject*() { return object; }
  PyObject* operator->() { return object; }
	PyObject* operator=(PyObject* object2) { object = object2; return object; }
	bool is() { return object ? true : false; }
	operator bool() { return object ? true : false; }
  
private:
  PyObject *object = nullptr;
};

#endif
