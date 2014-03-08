/* Large segments of this come from the gertboard test suite
 * Additional code and large sweeping changes to the original
 * are entirely to be blamed on me.
 */

/* RPI gpio hardware access */
#define GPIO_BASE         (0x20200000) /* GPIO controller */
/* 
#define BCM2708_PERI_BASE        (0x20000000)
#define GPIO_BASE                (BCM2708_PERI_BASE + 0x200000)
*/

#define PAGE_SIZE         (4*1024)
#define BLOCK_SIZE        (4*1024)

/* GPIO setup macros. Use INP_GPIO to clear values before OUT_GPIO or SET_GPIO_ALT. */
#define INP_GPIO(g)       *(gpio+((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g)       *(gpio+((g)/10)) |= (1<<(((g)%10)*3))
#define SET_GPIO_ALT(g,a) *(gpio+((g)/10)) |= (((a)<=3?(a)+4:(a)==4?3:2)<<(((g)%10)*3))

#define GPIO_SETR         *(gpio+7)  /* sets   bits which are 1 ignores bits which are 0 */
#define GPIO_CLRR         *(gpio+10) /* clears bits which are 1 ignores bits which are 0 */
#define GPIO_VALR         *(gpio+13)

#define GPIO_SET(g)       *(gpio+7)  &= (1<<(g)) /* sets   bits which are 1 ignores bits which are 0 */
#define GPIO_CLR(g)       *(gpio+10) &= (1<<(g)) /* clears bits which are 1 ignores bits which are 0 */

#define GPIO_READ(g)      *(gpio+13) & (1<<(g))
#define GPIO_GETMODE(g)   *(gpio+((g)/10)) & (1<<(((g)%10)*3))

#define GPIO_PULL         *(gpio+37) // Pull up/pull down
#define GPIO_PULLCLK0     *(gpio+38) // Pull up/pull down clock

/* RPI based names mapped to broadcom gpio numbers */
#define RPI_P1_03_R1 (1)
#define RPI_P1_05_R1 (2)
#define RPI_P1_07_R1 (4)
#define RPI_P1_08_R1 (14)
#define RPI_P1_10_R1 (15)
#define RPI_P1_11_R1 (17)
#define RPI_P1_12_R1 (18)
#define RPI_P1_13_R1 (21)
#define RPI_P1_15_R1 (22)
#define RPI_P1_16_R1 (23)
#define RPI_P1_18_R1 (24)
#define RPI_P1_19_R1 (10)
#define RPI_P1_21_R1 (9)
#define RPI_P1_22_R1 (25)
#define RPI_P1_23_R1 (11)
#define RPI_P1_24_R1 (8)
#define RPI_P1_26_R1 (7)

#define RPI_P1_03_R2 (3) /* changed */
#define RPI_P1_05_R2 (5) /* changed */
#define RPI_P1_07_R2 (4)
#define RPI_P1_08_R2 (14)
#define RPI_P1_10_R2 (15)
#define RPI_P1_11_R2 (17)
#define RPI_P1_12_R2 (18)
#define RPI_P1_13_R2 (27) /* changed */
#define RPI_P1_15_R2 (22)
#define RPI_P1_16_R2 (23)
#define RPI_P1_18_R2 (24)
#define RPI_P1_19_R2 (10)
#define RPI_P1_21_R2 (9)
#define RPI_P1_22_R2 (25)
#define RPI_P1_23_R2 (11)
#define RPI_P1_24_R2 (8)
#define RPI_P1_26_R2 (7)

#define RPI_P5_03_R2 (28)
#define RPI_P5_04_R2 (29)
#define RPI_P5_05_R2 (30)
#define RPI_P5_06_R2 (31)

void setup_io(lua_State *L);

/* Lua calls: */
int gpio_init(lua_State *L);
int gpio_read(lua_State *L);
int gpio_write(lua_State *L);
int gpio_setmode(lua_State *L);
int gpio_getmode(lua_State *L);
int gpio_setpullupdown(lua_State *L);
int gpio_readall(lua_State *L);

