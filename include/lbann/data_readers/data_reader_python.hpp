////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2014-2016, Lawrence Livermore National Security, LLC.
// Produced at the Lawrence Livermore National Laboratory.
// Written by the LBANN Research Team (B. Van Essen, et al.) listed in
// the CONTRIBUTORS file. <lbann-dev@llnl.gov>
//
// LLNL-CODE-697807.
// All rights reserved.
//
// This file is part of LBANN: Livermore Big Artificial Neural Network
// Toolkit. For details, see http://software.llnl.gov/LBANN or
// https://github.com/LLNL/LBANN.
//
// Licensed under the Apache License, Version 2.0 (the "Licensee"); you
// may not use this file except in compliance with the License.  You may
// obtain a copy of the License at:
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
// implied. See the License for the specific language governing
// permissions and limitations under the license.
////////////////////////////////////////////////////////////////////////////////

#ifndef LBANN_DATA_READERS_PYTHON_HPP_INCLUDED
#define LBANN_DATA_READERS_PYTHON_HPP_INCLUDED

#include "data_reader.hpp"
#ifdef LBANN_HAS_PYTHON
#include <Python.h>
#include <mutex>

namespace lbann {

namespace python {

/** Singleton class to manage embedded Python session.
 *
 *  This is very experimental. Be warned.
 */
class manager {
public:

  /** Get singleton instance. */
  static manager& get_instance();
  /** @brief Construct singleton instance.
   *  @detailed If there is already an instance, it is destroyed.
   */
  static void create();
  /** Destroy singleton instance. */
  static void destroy();

  /** Check if a Python error has occurred.
   *
   *  Throw an exception if an error is detected.
   *
   *  @param force_error Whether to force an exception to be thrown.
   */
  void check_error(bool force_error = false) const;

  /** RAII-style mutex wrapper.
   *
   *  Python session is not thread-safe.
   *
   *  @c std::lock_guard would be better than @c std::unique_lock, but
   *  it can't be returned from a function since it is non-copyable
   *  (guaranteed copy elision is C++17). @c std::scoped_lock is
   *  supposed to be strictly better than @c std::lock_guard, but it
   *  is C++17.
   */
  using mutex_guard_type = std::unique_lock<std::mutex>;
  /** RAII-style mutex wrapper.
   *
   *  Python session is not thread-safe.
   *
   *  A mutex is locked when this function is called and it is
   *  unlocked when the guard is destructed.
   */
  mutex_guard_type get_mutex_guard();

  ~manager();

private:

  /** Singleton instance. */
  static std::unique_ptr<manager> m_instance;
  /** Python session is not thread-safe. */
  std::mutex m_mutex;

  // Lifetime functions
  manager();
  manager(const manager&) = delete;
  manager operator=(const manager&) = delete;

};

/** Convenience wrapper around @c PyObject pointer.
 *
 *  This is very experimental. Be warned.
 */
class object {
public:
  object(PyObject* obj = nullptr);
  object(std::string val);
  object(El::Int val);
  object(DataType val);
  object(const object& other);
  object& operator=(const object& other);
  object(object&& other);
  object& operator=(object&& other);
  ~object();
  inline PyObject* get()                  { return m_ptr; }
  inline const PyObject* get() const      { return m_ptr; }
  inline operator PyObject*()             { return get(); }
  inline operator const PyObject*() const { return get(); }
private:
  PyObject* m_ptr;
};

} // namespace python

class python_reader : public generic_data_reader {
public:
  python_reader(std::string script,
                std::string sample_function,
                std::string num_samples_function,
                std::string sample_dims_function);
  python_reader(const python_reader&) = default;
  python_reader& operator=(const python_reader&) = default;
  ~python_reader() override = default;
  python_reader* copy() const override { return new python_reader(*this); }

  std::string get_type() const override {
    return "python_reader";
  }

  const std::vector<int> get_data_dims() const override;
  int get_num_labels() const override;
  int get_linearized_data_size() const override;
  int get_linearized_label_size() const override;

  void load() override;

protected:
  bool fetch_datum(CPUMat& X, int data_id, int mb_idx) override;
  bool fetch_label(CPUMat& Y, int data_id, int mb_idx) override;

private:
  std::vector<El::Int> m_sample_dims;
  El::Int m_num_samples;
  python::object m_sample_function;

};

} // namespace lbann

#endif // LBANN_HAS_PYTHON
#endif // LBANN_DATA_READERS_PYTHON_HPP_INCLUDED