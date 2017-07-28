#!/bin/sh

# You can submit this with something like:
# sbatch -Abrain -N16 --enable-hyperthreads -t 1440 --clear-ssd --msr-safe --output="slurm-lbann-<scriptname>-test-%j.out" tests/<scriptname>.sh
# and can include the working directory with:
# --workdir=/g/g19/vanessen/DeepLearning/lbann.git/lbann_v0.2/examples 

#SBATCH --time=1440

TESTDIR=`dirname $0`
DIRNAME=`dirname $TESTDIR`

FULLSCRIPT=.
SCRIPT=../build/catalyst.llnl.gov/model_zoo/lbann

if [ -e "${DIRNAME}/${SCRIPT}" ] ; then
    FULLSCRIPT="${DIRNAME}/${SCRIPT}"
fi

if [ ! -z "$SLURM_SUBMIT_DIR" ] ; then
  if [ -e "${SLURM_SUBMIT_DIR}/${SCRIPT}" ] ; then
      FULLSCRIPT="${SLURM_SUBMIT_DIR}/${SCRIPT}"
  fi
fi

echo "Executing script $0 -> ${SLURM_JOB_NAME}"
echo "Clearing /l/ssd for batch execution"
srun -N${SLURM_NNODES} --clear-ssd hostname

STD_OPTS="--model=../model_zoo/prototext/model_mnist_partitioned_io.prototext --reader=../model_zoo/prototext/data_reader_mnist.prototext --optimizer=../model_zoo/prototext/opt_adagrad.prototext"
echo "################################################################################"
for b in 300 150 100 75 60 50; do
  for k in 1 2 3 4 5 6; do
    CMD="srun -n$((${k}*${MAX_MB}/${b})) ${FULLSCRIPT} ${STD_OPTS} --mini_batch_size=${b} --num_epochs=5 --procs_per_model=${k}"
    echo "${CMD}"
    ${CMD}
    echo "################################################################################"
  done
done
