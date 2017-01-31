import java.util.concurrent.locks.ReentrantLock;

class BetterSafe implements State {
    private byte[] value;
    private final byte maxval;
    private final ReentrantLock threadLock  = new ReentrantLock();

    BetterSafe(byte[] v) { value = v; maxval = 127; }

    BetterSafe(byte[] v, byte m) { value = v; maxval = m; }

    public int size() { return value.length; }

    public byte[] current() { return value; }

    public boolean swap(int i, int j) {
        threadLock.lock();
    if (value[i] <= 0 || value[j] >= maxval) {
        threadLock.unlock();
        return false;
    }
    value[i]--;
    value[j]++;
    threadLock.unlock();
    return true;
    }
}