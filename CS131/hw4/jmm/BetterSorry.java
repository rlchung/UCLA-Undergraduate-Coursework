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
