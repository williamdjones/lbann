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

#include "lbann/layers/lbann_input_layer.hpp"
#include "lbann/utils/lbann_exception.hpp"
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

using namespace std;
using namespace El;

lbann::input_layer::input_layer(lbann_comm* comm, uint mini_batch_size, DataReader *training_data_reader, DataReader *testing_data_reader, std::vector<regularizer*> regs)
  : io_layer(comm, mini_batch_size, training_data_reader, testing_data_reader)
{
  NumNeurons = io_layer::get_linearized_data_size();
}

lbann::input_layer::input_layer(lbann_comm* comm, uint mini_batch_size, DataReader *training_data_reader)
  : input_layer(comm, mini_batch_size, training_data_reader, NULL, {}) {}

/**
 * Input layers are not able to return output matrices for backward propagation
 */
DistMat *lbann::input_layer::bp_output() {
  return NULL;
}