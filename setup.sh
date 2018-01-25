if [ "$0" != "-bash" ]
then
    # best for calling from zsh                                                                                                                                                                                                                                                   
    export XHAL_ROOT=$(dirname $(readlink -f "$0") )
else
    # bash specific, must be sourced from a bash shell...                                                                                                                                                                                                                         
    export XHAL_ROOT=$(dirname "${BASH_SOURCE[0]}")
fi

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$XHAL_ROOT/lib
export PATH=$PATH:$XHAL_ROOT/python/reg_interface
