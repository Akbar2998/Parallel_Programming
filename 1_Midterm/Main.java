import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

class Gate {
    private Lock lock = new ReentrantLock();
    private Condition canCross = lock.newCondition();
    private int widthAvailable = 9;

    public void crossGate(int width) throws InterruptedException {
        lock.lock();
        try {
            while (widthAvailable < width) {
                canCross.await();
            }
            widthAvailable -= width;
            System.out.println(Thread.currentThread().getName() + " is crossing the GATE with width " + width);
            Thread.sleep(550);
            System.out.println(Thread.currentThread().getName() + " has crossed the GATE.");
            widthAvailable += width;
            canCross.signalAll();
        }
        //unlocking
        finally {

            lock.unlock();
        }
    }
}
//Here we create Person class with with
class Person extends Thread {
    private int width;
    private Gate gate;

    public Person(int width, Gate gate) {
        this.width = width;
        this.gate = gate;
    }

    @Override
    public void run() {
        try {
            gate.crossGate(width);
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
    }
}

public class Main {
    public static void main(String[] args) {
        Gate gate = new Gate();
        // just a cyclee for the random and counting
        for (int i = 0; i < 26; i++) {
            int width = (Math.random() < 0.5) ? 1 : 2;  // That's Random for the with
            Thread person = new Person(width, gate);
            person.setName("Person " + (i + 1));
            person.start();
        }
    }
}
