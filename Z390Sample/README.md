# Delivery 2021/06/21

## Description

The delivery contains example code usage of black zone printing.

### Usage of Rlr;p command

```cpp
evolis_t*   printer;

if ((printer = evolis_open2("usb:///dev/usb/lp0", EVOLIS_TY_EVOLIS)) != NULL) {
    char cmd[32];
    char buffer[1024];

    strcpy(cmd, "Rlr;p");
    evolis_command(printer, cmd, sizeof (cmd), buffer, sizeof buffer);
    printf("> cmd=%4s buffer=%s\n", cmd, buffer); // Answer = OK

    evolis_insert(printer);

    evolis_command(printer, cmd, sizeof (cmd), buffer, sizeof buffer);
    printf("> cmd=%4s buffer=%s\n", cmd, buffer); // Answer = CARD

    evolis_eject(printer);
    evolis_close(printer);
} else {
    printf("Error: %s - %s\n", evolis_last_error_string(), strerror(errno));
}
```

When you send Rlr;p command, answer is OK. After insert card, the answer is CARD.

### Usage of print option for black zone print

```cpp
evolis_t*   printer;

if ((printer = evolis_open2("usb:///dev/usb/lp0", EVOLIS_TY_EVOLIS)) != NULL) {
    evolis_print_init(printer);

    evolis_print_set_option(printer, "GRibbonType", "RC_YMCKO");
    evolis_print_set_option(printer, "FBlackManagement", "TEXTINBLACK");
    evolis_print_set_option(printer, "IFTextRegion", "0x0x1000x220");
    evolis_print_set_option(printer, "IFBlackLevelValue", "40");
    evolis_print_set_option(printer, "IFDarkLevelValue ", "10");
    evolis_print_set_imagep(printer, EVOLIS_FA_FRONT, "resources/source.bmp");
    evolis_print_exec(printer);
    evolis_close(printer);
} else {
    printf("Error: %s - %s\n", evolis_last_error_string(), strerror(errno));
}
```

Before start print, send print option for print black zone.

The source.bmp :

![image source](./source.bmp)