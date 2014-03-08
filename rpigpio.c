
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#include <lua5.1/lua.h>
#include <lua5.1/lualib.h>
#include <lua5.1/lauxlib.h>

#include "rpigpio.h"


// I/O access
volatile unsigned int *gpio = NULL;

static const struct luaL_reg rpigpio[] = {
  {"init", gpio_init},
  {"read", gpio_read},
  {"write", gpio_write},
  {"setmode", gpio_setmode},
  {"getmode", gpio_getmode},
  {"setpullupdown", gpio_setpullupdown},
  {"readall", gpio_readall},
  {NULL, NULL}
};

int luaopen_rpigpio(lua_State *L) {
  luaL_register(L, "rpigpio", rpigpio);
  return 1;
}



/*
 * gpio.init(revision)
 * revision: 1, 2
 */
int gpio_init(lua_State *L) {
  long rev = luaL_checkinteger(L, 1); // get item 1
  if (!gpio) setup_io(L);
  return 1; // number of items returned
}

/*
 * gpio.read(pin)
 * returns 0 or 1
 */
int gpio_read(lua_State *L) {
  long pin = luaL_checkinteger(L, 1); // get item 1
  if (!gpio) luaL_error(L, "Successful gpio_init() required.");
  lua_pushboolean(L, GPIO_READ(pin)?1:0); 
  return 1; // number of items returned
}

/*
 * gpio.write(pin,value)
 * sets (1) or clears (0) a pin.
 */
int gpio_write(lua_State *L) {
  long pin = luaL_checkinteger(L, 1);
  long val = luaL_checkinteger(L, 2);
  if (!gpio) luaL_error(L, "Successful gpio_init() required.");
  if (val)
    GPIO_SET(pin);
  else
    GPIO_CLR(pin);
  return 0;
}

/*
 * gpio.setmode(pin,mode)
 * Modes:
 *   0 input
 *   1 output
 */
int gpio_setmode(lua_State *L) {
  long pin = luaL_checkinteger(L, 1);
  long mode = luaL_checkinteger(L, 2);
  if (!gpio) luaL_error(L, "Successful gpio_init() required.");
  INP_GPIO(pin);
  if (mode) OUT_GPIO(pin);
  return 0;
}

/*
 * gpio.getmode(pin)
 * Modes:
 *   0 input
 *   1 output
 */
int gpio_getmode(lua_State *L) {
  long pin = luaL_checkinteger(L, 1);
  if (!gpio) luaL_error(L, "Successful gpio_init() required.");
  lua_pushboolean(L, GPIO_GETMODE(pin)?1:0);
  return 1; // number of items returned
}

/*
 * gpio.setpullupdown(pin,mode)
 * Modes:
 *   0 reset (floating)
 *   1 pull down
 *   2 pull up
 */
int gpio_setpullupdown(lua_State *L) {
  long pin = luaL_checkinteger(L, 1);
  long mode = luaL_checkinteger(L, 2);
  if (!gpio) luaL_error(L, "Successful gpio_init() required.");

  GPIO_PULL = mode;
  usleep(5);
  GPIO_PULLCLK0 = (1<<pin); // FIXME. This may be fubar.
  usleep(5);
  GPIO_PULL = 0;
  GPIO_PULLCLK0 = 0;
  return 0;
}

/*
 * gpio.readall()
 * returns a table, IN
 */
int gpio_readall(lua_State *L) {
  unsigned int pin = 0;
  unsigned int dump = GPIO_VALR;
  if (!gpio) luaL_error(L, "Successful gpio_init() required.");
  lua_newtable(L);
  for (pin=0;pin<32;pin++) {
    lua_pushfstring(L, "gpio%d", pin);
    lua_pushboolean(L, dump & (1<<pin));
    lua_rawset(L, -3);
  }
  return 1;
}



/*
 * Set up a memory region to access GPIO
 */
void setup_io(lua_State *L)
{
   if (gpio) return;

   int  mem_fd;
   static void *gpio_map;
   /* open /dev/mem */
   if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0) {
      luaL_error(L, "Can't open /dev/mem");
      gpio_map = NULL;
      gpio = NULL;
   }

   /* mmap GPIO */
   gpio_map = mmap(
      NULL,             //Any adddress in our space will do
      BLOCK_SIZE,       //Map length
      PROT_READ|PROT_WRITE,// Enable reading & writting to mapped memory
      MAP_SHARED,       //Shared with other processes
      mem_fd,           //File to map
      GPIO_BASE         //Offset to GPIO peripheral
   );

   close(mem_fd); //No need to keep mem_fd open after mmap

   if (gpio_map == MAP_FAILED) {
      luaL_error(L, "mmap error: %d", (int)gpio_map);
      gpio_map = NULL;
      gpio = NULL;
   }

   // Always use volatile pointer!
   gpio = (volatile unsigned *)gpio_map;
} // setup_io

