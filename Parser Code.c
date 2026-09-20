// Parser code to send 'S', 'L', or 'R' over serial to Arduino
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

#define RIGHT_THRESHOLD 0.6
#define LEFT_THRESHOLD -0.6

int main() {
    // Adjust this if your Arduino is on a different COM port
    int fd = open("/dev/ttyS4", O_RDWR | O_NOCTTY);
    if (fd == -1) {
        perror("Could not open serial port");
        return 1;
    }

    struct termios options;
    tcgetattr(fd, &options);
    cfsetspeed(&options, B9600);  // Must match Arduino Serial.begin()
    options.c_cflag |= (CLOCAL | CREAD);
    tcsetattr(fd, TCSANOW, &options);

    // Send start signal to Arduino
    write(fd, "S", 1);

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), stdin)) {
        int t, T, c, X, S;
        double gx, gy, gz, lx, ly, rx, ry;

        if (sscanf(buffer, "%d %lf %lf %lf %d %d %d %lf %lf %lf %lf", 
                   &t, &gx, &gy, &gz, &T, &c, &X, &lx, &ly, &rx, &ry) == 11) {

            if (rx > RIGHT_THRESHOLD) {
                write(fd, "R", 1);
            } else if (rx < LEFT_THRESHOLD) {
                write(fd, "L", 1);
            }
        }
    }

    close(fd);
    return 0;
}
