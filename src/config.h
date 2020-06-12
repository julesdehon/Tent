#ifndef CONFIG_H
#define CONFIG_H

#include "map.h"
#include "variable.h"

typedef map_t(Variable*) Config;

Config* load_config();

#endif
