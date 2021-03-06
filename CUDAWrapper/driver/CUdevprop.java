
package CUDAWrapper.driver;

import java.util.Arrays;

/**
 * Legacy device properties.
 *
 * @see CUDAWrapperDriver#cuDeviceGetProperties(CUdevprop, CUdevice)
 */
public class CUdevprop
{
    /**
     * The maximum number of threads per block;
     */
    public int maxThreadsPerBlock;

    /**
     * The maximum sizes of each dimension of a block;
     */
    public int maxThreadsDim[] = new int[3];

    /**
     * The maximum sizes of each dimension of a grid;
     */
    public int maxGridSize[] = new int[3];

    /**
     * The total amount of shared memory available per block in bytes;
     */
    public int sharedMemPerBlock;

    /**
     * The total amount of constant memory available on the device in bytes;
     */
    public int totalConstantMemory;

    /**
     * The warp size;
     */
    public int SIMDWidth;

    /**
     * The maximum pitch allowed by the memory copy functions that involve memory regions allocated through cuMemAllocPitch();
     */
    public int memPitch;

    /**
     * The total number of registers available per block;
     */
    public int regsPerBlock;

    /**
     * The clock frequency in kilohertz;
     */
    public int clockRate;

    /**
     * The alignment requirement; texture base addresses that are aligned to textureAlign bytes do not need an offset applied to texture fetches
     */
    public int textureAlign;


    /**
     * Creates a new, uninitialized CUdevprop
     */
    public CUdevprop()
    {
    }

    /**
     * Returns a String representation of this object.
     *
     * @return A String representation of this object.
     */
    @Override
    public String toString()
    {
        return "CUdevprop["+createString(",")+"]";
    }

    /**
     * Creates and returns a formatted (aligned, multi-line) String
     * representation of this object
     *
     * @return A formatted String representation of this object
     */
    public String toFormattedString()
    {
        return "Device properties:\n    "+createString("\n    ");
    }

    /**
     * Creates and returns a string representation of this object,
     * using the given separator for the fields
     *
     * @return A String representation of this object
     */
    private String createString(String f)
    {
        return
            "maxThreadsPerBlock="+maxThreadsPerBlock+f+
            "maxThreadsDim="+Arrays.toString(maxThreadsDim)+f+
            "maxGridSize="+Arrays.toString(maxGridSize)+f+
            "sharedMemPerBlock="+sharedMemPerBlock+f+
            "totalConstantMemory="+totalConstantMemory+f+
            "regsPerBlock="+regsPerBlock+f+
            "SIMDWidth="+SIMDWidth+f+
            "memPitch="+memPitch+f+
            "regsPerBlock="+regsPerBlock+f+
            "clockRate="+clockRate+f+
            "textureAlign="+textureAlign;
    }
}
