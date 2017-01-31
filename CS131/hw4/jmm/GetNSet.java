import java.util.concurrent.atomic.AtomicIntegerArray;

class GetNSet implements State {
    private AtomicIntegerArray value;
    private byte maxval;

    GetNSet(byte[] v) { 
        value = new AtomicIntegerArray(v.length);
        for (int i = 0; i < v.length; i++)
            value.set(i, (int) v[i]);
        maxval = 127; }

    GetNSet(byte[] v, byte m) { 
        value = new AtomicIntegerArray(v.length);
        for (int i = 0; i < v.length; i++)
            value.set(i, (int) v[i]);
        maxval = m; }

    //Okay; Doesn't need modification
    public int size() { return value.length(); }

    //Todo: Change to volatile access
    public byte[] current() { 
        //reverse GetNSet code to store value into int array
        //int[] temp_array3 = new int[value.length()];
        byte[] ret_list = new byte [value.length()];
        for (int i = 0; i < value.length(); i++)
        {
            //temp_array3[i] = value.get(i);
            ret_list[i] = (byte) value.get(i);
        } 
        return ret_list; }

    public boolean swap(int i, int j) {
	if (value.get(i) <= 0 || value.get(j) >= maxval) {
	    return false;
	}

	value.set(i, value.get(i)-1);
	value.set(j, value.get(j)+1);
	return true;
    }
}
