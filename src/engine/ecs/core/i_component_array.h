﻿#pragma once
#include "../../globals.h"
class i_component_array
{
public:
    virtual ~i_component_array() = default;
    virtual void entity_destroyed(entity entity) = 0;
};
