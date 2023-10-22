import java.util.Random;

public class Main {
    public static class Bridge {
        private int currentLoad = 0;
        private static final int maxCapacity = 12;

        public synchronized void enter(int load) {
            while (currentLoad + load > maxCapacity) {
                try {
                    wait();
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                }
            }
            currentLoad += load;
        }

        public synchronized void leave(int load) {
            currentLoad -= load;
            notify();
        }
    }

    public static class Soldier extends Thread {
        private int load;
        private Bridge bridge;

        public Soldier(int load, Bridge bridge) {
            this.load = load;
            this.bridge = bridge;
        }

        @Override
        public void run() {
            System.out.println("Soldier with load " + load + " is ready to cross the bridge.");

            bridge.enter(load);

            try {
                Thread.sleep(490); // Simulate the time it takes to cross the bridge
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }

            System.out.println("Soldier with load " + load + " has crossed the bridge.");
            bridge.leave(load);
        }
    }

    public static void main(String[] args) {
        Bridge bridge = new Bridge();
        Random random = new Random();

        for (int i = 0; i < 31; i++) {
            int load = random.nextInt(5) + 3;
            Soldier soldier = new Soldier(load, bridge);
            soldier.start();
        }
    }
}
