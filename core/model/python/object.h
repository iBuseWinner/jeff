#ifndef PYTHON_OBJECT_H
#define PYTHON_OBJECT_H

#pragma push_macro("slots")
#undef slots
#include <Python.h>
#pragma pop_macro("slots")

/*!
 * @class Object
 * @brief Helps manage memory when using Python objects.
 */
class Object {
public:
  Object(PyObject *_o) : o(_o) {}
  ~Object() { if (o) Py_CLEAR(o); }
  
  operator PyObject*() { return o; }
  PyObject* operator->() { return o; }
	PyObject* operator=(PyObject* o2) { o = o2; return o; }
	PyObject* pure() { return o; }
	bool is() { return o ? true : false; }
	operator bool() { return o ? true : false; }
  
private:
  PyObject *o = nullptr;
};

#endif
