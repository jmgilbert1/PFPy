# Testing reading in a PFB with a C program using ctypes

import numpy as np
import numpy.ctypeslib as npct
#from ctypes import CDLL, POINTER, c_double, byref
import ctypes
from numpy.ctypeslib import ndpointer

def read_pfb(fp):
    # load the library, using numpy mechanisms
    #libcd = npct.load_library("readpfb2.dylib", ".")

    dll = ctypes.cdll.LoadLibrary(r'/u/st/fl/jagilber/scratch/Utilities/Python_C_PfbReader/readpfb2.so')

    #N = (270*220*17)
    #a = np.empty((270, 220, 17))
    #a = (ctypes.c_double * N)()
    #indata = np.ascontiguousarray(a, dtype=np.float64)
    indata = np.ones((220, 270, 17), dtype=np.double)
    outdata = np.zeros((17, 270, 220), dtype=np.double)
    #indata = np.zeros(N).astype(np.float64)
    #print(indata.flags['C_CONTIGUOUS'])
    #a = np.arange(N, dtype=np.float64)
    #aptr = a.ctypes.data_as(ctypes.POINTER(ctypes.c_double))
    #indata2 = indata.ctypes.data_as(ctypes.c_void_p)
    #indata2 = indata.ctypes.data_as(ctypes.POINTER(ctypes.c_double))
    #dll.readPFB.argtypes = [ctypes.POINTER(ctypes.c_double), ctypes.c_long, ctypes.c_char_p]
    #dll.freeme.argtypes = ctypes.c_void_p,
    #dll.freeme.restype = None
    #fp = r'/Users/jgilbert/ParFlow/parflow/CV_Model_JG/5_Lyr_Subsurface/20150202_CLMSpinUp10/PFB_Results/CV_5lyr_CLMSpinUp10.out.press.00000.pfb'
    #data1 = POINTER(c_double)()

    #dll.readPFB.argtypes = [indata2, ctypes.c_char_p]
    #dll.readPFB.restype = None
    dll.readPFB.argtypes = [ndpointer(ctypes.c_double, flags="C_CONTIGUOUS"), ndpointer(ctypes.c_double), ctypes.c_char_p]
    dll.readPFB.restype = None
    #dll.readPFB(ctypes.c_void_p(indata.ctypes.data), fp)
    dll.readPFB(indata,outdata, fp)
    #dll.readPFB(byref(data1))

    #dll.readPFB(aptr, N, fp)

    a2 = outdata.reshape([17, 220, 270], order='C')
    #print("deleting pointer var")
    #dll.freeme(indata2)
    #del(aptr)
    #del(a)
    #print("Made it here..")
    #print(np.max(a2))
    return(a2)
    # setup the return typs and argument types
    #libcd.main.restype = None
    #libcd.main.argtypes = None

    #def read_pfb():
    #    return libcd.main()
