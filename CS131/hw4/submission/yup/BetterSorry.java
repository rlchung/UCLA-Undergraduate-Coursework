/*import java.util.concurrent.atomic.AtomicIntegerArray;

class BetterSorry implements State {
    private AtomicIntegerArray value;
    private final byte maxval;
    private final int input_length;

    BetterSorry(byte[] v) {
        input_length = v.length; 
        value = new AtomicIntegerArray(input_length);
        for (int i = 0; i < input_length; i++)
            value.getAndSet(i, (int) v[i]);
        maxval = 127; }

    BetterSorry(byte[] v, byte m) { 
        input_length = v.length;
        value = new AtomicIntegerArray(input_length);
        for (int i = 0; i < input_length; i++)
            value.getAndSet(i, (int) v[i]);
        maxval = m; }

    //Okay; Doesn't need modification
    public int size() { return input_length; }

    public byte[] current() { 
        //reverse GetNSet code to store value into int array
        //int[] temp_array3 = new int[value.length()];
        byte[] ret_list = new byte [input_length];
        for (int i = 0; i < input_length; i++)
        {
            //temp_array3[i] = value.get(i);
            ret_list[i] = (byte) value.get(i);
        } 
        return ret_list; }

    public boolean swap(int i, int j) {
    if (value.get(i) <= 0 || value.get(j) >= maxval) {
        return false;
    }

    value.getAndDecrement(i);
    value.getAndIncrement(j);

    return true;
    }
}*/

import java.util.concurrent.locks.ReentrantLock;

class BetterSorry implements State {
    private byte[] value;
    private byte maxval;
    private ReentrantLock[] lock_array ;
    private final int input_length;

    BetterSorry(byte[] v) { 
        value = v; 
        maxval = 127;
        input_length = v.length;
        lock_array = new ReentrantLock[input_length];
        for (int i = 0; i < input_length; i++)
            lock_array[i] = new ReentrantLock();
    }

    BetterSorry(byte[] v, byte m) { 
        value = v; 
        maxval = m;
        input_length = v.length;
        lock_array = new ReentrantLock[input_length];
        for (int i = 0; i < input_length; i++)
            lock_array[i] = new ReentrantLock();
    }

    public int size() { return input_length; }

    public byte[] current() { return value; }

    public boolean swap(int i, int j) {
    if (value[i] <= 0 || value[j] >= maxval) {
        return false;
    }

    if (lock_array[i].isHeldByCurrentThread() && lock_array[j].isHeldByCurrentThread()) {
        value[i]--;
        value[j]++;
    }
    return true;
    }
}
