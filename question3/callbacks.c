#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    char name[30];
    int type;

    union {
        float temperature; // 0=temp
        int pressure; // 1=pressure
        float voltage; // 2=voltage
    } reading;

} Device;

typedef void (*device)(Device *);

// temp monitor
void temp_monitor(Device *d) {
    printf("%s Temperature: %.2f°C\n",
           d->name, d->reading.temperature);
}

// pressure monitor
void pressure_monitor(Device *d) {
    printf("%s Pressure: %d Pa\n",
           d->name, d->reading.pressure);
}

// voltage monitor
void volt_monitor(Device *d) {
    printf("%s Voltage: %.2f V\n",
           d->name, d->reading.voltage);
}

// process device
void process_device(Device *d, device call) {
    call(d);
}

int main(void) {
    srand(time(NULL));

    Device *devices = malloc(10 * sizeof(Device));
    if (!devices) {
        printf("Memory error\n");
        return 1;
    }

    // create 3 devices repeated
    for (int i = 0; i < 10; i++) {

        if (i % 3 == 0) {
            snprintf(devices[i].name, 30, "TempSensor_%d", i);
            devices[i].type = 0;
            devices[i].reading.temperature = (rand() % 4000) / 100.0;
        }
        else if (i % 3 == 1) {
            snprintf(devices[i].name, 30, "PressureSensor_%d", i);
            devices[i].type = 1;
            devices[i].reading.pressure = (rand() % 2000) + 800;
        }
        else {
            snprintf(devices[i].name, 30, "VoltageSensor_%d", i);
            devices[i].type = 2;
            devices[i].reading.voltage = (rand() % 2400) / 100.0;
        }
    }

    printf("Device Monitoring Simulator\n");
    printf("Memory address: %p\n\n", (void *)devices);

    // simulate readings using callbacks
    for (int i = 0; i < 10; i++) {

        if (devices[i].type == 0) {
            process_device(&devices[i], temp_monitor);
        }
        else if (devices[i].type == 1) {
            process_device(&devices[i], pressure_monitor);
        }
        else {
            process_device(&devices[i], volt_monitor);
        }
    }

    free(devices);

    printf("\nSimulation complete.\n");
    return 0;
}

