declare module 'wiringpi-sx' {
    
    /**
     * @description Returns the version of the used native wiringpi library (libwiringPi)
     * @returns {string} version as string major.minor
     * @throws ERR_WPI_RUNTIME 
     */
    export function libwiringPiVersion (): string;
    
    /**
     * @description Initialises wiringPi and assumes that the calling program is going to be using the wiringPi pin numbering scheme.
     *    This is a simplified numbering scheme which provides a mapping from virtual pin numbers 0 through 63 to the real underlying Broadcom GPIO pin numbers.
     *    see the pins page (http://wiringpi.com/pins/) for a table
     *    which maps the wiringPi pin number to the Broadcom GPIO pin number to the physical location on the edge connector.
     *    This function needs to be called with root privileges.
     * @param {string} mode use 'wpi' to call the native library function wiringPiSetup()
     * @returns {number}  error code if v1 mode otherwise always returns 0
     * @throws ERR_WPI_RUNTIME, ERR_WPI_LOGICERROR, ERR_WPI_EXECUTIONERROR
     */
    export function setup (mode: 'wpi'): number;

    
    /**
     * @description This sets the mode of a pin to either INPUT, OUTPUT, PWM_OUTPUT or GPIO_CLOCK.
     *     This function has no effect when in Sys mode. Note that usage of pin modes is restricted.
     *     If you need to change the pin mode, then you can do it with the gpio program in a script before you start your program.
     * @param {number} pin use the virtual pin number 0 to 63 (see http://wiringpi.com/pins/)
     * @param {number} mode use the constants INPUT, OUTPUT, PWM_OUTPUT or GPIO_CLOCK.
     * @throws ERR_WPI_RUNTIME, ERR_WPI_LOGICERROR
     */
    export function pinMode (pin: number, mode: number): void;

    // Library function: void digitalWrite (int pin, int value)
    // Description :
    //    

    /**
     * @description This sets the pull-up or pull-down resistor mode on the given pin, which should be set as an input.
     *     Unlike the Arduino, the BCM2835 has both pull-up an down internal resistors. 
     *     This function has no effect when in Sys mode. If you need to activate a pull-up/pull-down, then you can do
     *     it with the gpio program in a script before you start your program.
     * @param {number} pin use the virtual pin number 0 to 63 (see http://wiringpi.com/pins/)
     * @param {number} pud use PUD_OFF (no pull up/down), PUD_DOWN (pull to ground) or PUD_UP (pull to 3.3v)
     * @throws ERR_WPI_RUNTIME, ERR_WPI_LOGICERROR
     */
    export function pullUpDnControl (pin: number, pud: number): void;

    /**
     * @description Write the value HIGH or LOW (1 or 0) to the given pin which must have been previously set as an output.
     *     WiringPi treats any non-zero number as HIGH, however 0 is the only representation of LOW. 
     * @param {number} pin use the virtual pin number 0 to 63 (see http://wiringpi.com/pins/)
     * @param {number} value use the constants LOW or HIGH
     * @throws ERR_WPI_RUNTIME, ERR_WPI_LOGICERROR
     */
    export function digitalWrite (pin: number, value: number): void;

    /**
     * @description Read the value of the given pin.
     * @param {number} pin use the virtual pin number 0 to 63 (see http://wiringpi.com/pins/)
     * @returns {number} value of the pin (HIGH or LOW)
     * @throws ERR_WPI_RUNTIME, ERR_WPI_LOGICERROR
     */
    export function digitalRead (pin: number): number;

    /**
     * @description Set the freuency on a GPIO clock pin.
     *     Don't forget to set correct pin mode: pinMode(7, GPIO_CLOCK)
     *     Setting wrong frequency value causes break down of SSH, ... (9500000 works)
     * @param {number} pin use the virtual pin number 0 to 63 (see http://wiringpi.com/pins/)
     * @param {number} frequency value of frequency in Hz.
     * @throws ERR_WPI_RUNTIME, ERR_WPI_LOGICERROR
     */
    export function gpioClockSet (pin: number, frequency: number): void;

    /**
     * @description Initialize the desired SPI channel with CPOL=0 and CPHA=0.
     * @param {number} channel use value 0 or 1 to select the SPI channel
     * @param {number} speed use values between 500000 and 32000000 for the SPI clock frequency in Hz
     * @returns {number} file-descriptor of the SPI device
     * @throws ERR_WPI_RUNTIME, ERR_WPI_LOGICERROR, ERR_WPI_EXECUTIONERROR
     */    
    export function wiringPiSPISetup (channel: 0 | 1, speed: number): number;

    /**
     * @description Initialize the desired SPI channel with the desired operation mode (CPOL/CPHA).
     * @param {number} channel use value 0 or 1 to select the SPI channel
     * @param {number} speed use values between 500000 and 32000000 for the SPI clock frequency in Hz
     * @param {number} mode use 0, 1, 2 or 3. Bit 0 is CPOL, bit 1 is CPHA.
     * @returns {number} file-descriptor of the SPI device
     * @throws ERR_WPI_RUNTIME, ERR_WPI_LOGICERROR, ERR_WPI_EXECUTIONERROR
     */
    export function wiringPiSPISetupMode (channel: 0 | 1, speed: number, mode: 0 | 1 | 2 | 3): number;

    /**
     * @description Return the file-descriptor for the given channel
     * @param {number} channel use value 0 or 1 to select the SPI channel
     * @returns {number} file descriptor of the SPI device
     * @throws ERR_WPI_RUNTIME, ERR_WPI_LOGICERROR, ERR_WPI_EXECUTIONERROR
     */
    export function wiringPiSPIGetFd (channel: 0 | 1): number;

    /**
     * @description Write and Read a block of data over the SPI bus.
     *     Note the data ia being read into the transmit buffer, so will overwrite it!
     * @param {number} channel use value 0 or 1 to select the SPI channel
     * @param {Buffer} data binary data stream to write and read data. 
     * @returns {number} number of transferred bytes
     * @throws ERR_WPI_RUNTIME, ERR_WPI_LOGICERROR, ERR_WPI_EXECUTIONERROR
     */
    export function wiringPiSPIDataRW (channel: 0 | 1, data: Buffer): number;

    /**
     * @description This closes opened SPI file descriptor.
     * @param {number} fd file-descriptor returned either from wiringPiSPISetup or wiringPiSPISetupMode
     * @throws ERR_WPI_RUNTIME, ERR_WPI_LOGICERROR, ERR_WPI_EXECUTIONERROR
     */
    export function wiringPiSPIClose (fd: number): void;


    // *************************************************************************************************
    // Serial
    // *************************************************************************************************

    /**
     * @description This opens and initialises the serial device and sets the baud rate.
     *     It sets the port into “raw” mode (character at a time and no translations), and sets the read timeout to 10 seconds.
     *     You can use the standard read(), write(), etc. system calls on this file descriptor as required.
     *     E.g. you may wish to write a larger block of binary data where the serialPutchar() or serialPuts() function
     *     may not be the most appropriate function to use, in which case, you can use write() to send the data.
     *     Example: serialOpen('/dev/ttyAMA0', 9600);
     * @param {string} device name of serial device
     * @param {number} baudrate baudrate used by this serial device
     * @returns {number} file-descriptor of serial device
     * @throws ERR_WPI_RUNTIME, ERR_WPI_LOGICERROR, ERR_WPI_EXECUTIONERROR
     */
    export function serialOpen (device: string, baudrate: number): number;

    /**
     * @description Closes the device identified by the file descriptor given.
     * @param {number} fd file-descriptor of serial device
     * @throws ERR_WPI_RUNTIME, ERR_WPI_LOGICERROR, ERR_WPI_EXECUTIONERROR
     */
    export function serialClose (fd: number): void;

    /**
     * @description This discards all data received, or waiting to be send down the given device.
     * @param {number} fd file-descriptor of serial device
     * @throws ERR_WPI_RUNTIME, ERR_WPI_LOGICERROR, ERR_WPI_EXECUTIONERROR
     */
    export function serialFlush (fd: number): void;

    /**
     * @description Sends the single byte to the serial device identified by the given file descriptor.
     * @param {number} fd file-descriptor of serial device
     * @param {number} character the character value (0 to 255) send via the serial device.
     * @throws ERR_WPI_RUNTIME, ERR_WPI_LOGICERROR, ERR_WPI_EXECUTIONERROR
     */
    export function serialPutChar (fd: number, character: number): void;

    /**
    * @description Sends the nul-terminated string to the serial device identified by the given file descriptor.
    * @param {number} fd file-descriptor of serial device
    * @param {number} data the string send via the serial device.
    * @throws ERR_WPI_RUNTIME, ERR_WPI_LOGICERROR, ERR_WPI_EXECUTIONERROR
    */
    export function serialPuts (fd: number, data: string): number;

    /**
    * @description Sends the nul-terminated string to the serial device identified by the given file descriptor.
    * @param {number} fd file-descriptor of serial device
    * @param {number} data the string send via the serial device.
    * @throws ERR_WPI_RUNTIME, ERR_WPI_LOGICERROR, ERR_WPI_EXECUTIONERROR
    */
    export function serialPrintf (fd: number, data: string): number;

    /**
    * @description Returns the number of characters available for reading.
    * @param {number} fd file-descriptor of serial device
    * @returns {number} number of bytes available
    * @throws ERR_WPI_RUNTIME, ERR_WPI_LOGICERROR, ERR_WPI_EXECUTIONERROR
    */
    export function serialDataAvail (fd: number): number;

    /**
    * @description Returns the next character available on the serial device.
    *     This call will block for up to 10 seconds if no data is available.
    * @param {number} fd file-descriptor of serial device
    * @returns {number} received character value 0 to 255, or -1 if no value is received.
    * @throws ERR_WPI_RUNTIME, ERR_WPI_LOGICERROR, ERR_WPI_EXECUTIONERROR
    */
    export function serialGetchar (fd: number): number;


    // Pin modes defined in native wiringpi lib
    export const INPUT: number;
    export const OUTPUT: number;
    export const PWM_OUTPUT: number;
    export const GPIO_CLOCK: number;
    export const SOFT_PWM_OUTPUT: number;
    export const SOFT_TONE_OUTPUT: number;
    export const PWM_TONE_OUTPUT: number;

    export const PUD_OFF: number;
    export const PUD_DOWN: number;
    export const PUD_UP: number;

    // Version of this node.js module
    export const VERSION: string;
}
