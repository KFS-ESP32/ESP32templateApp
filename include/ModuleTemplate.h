/*
 * Copyright (C) 08/10/2024 Karlfried S.
 */
#pragma once

#include <TaskSchedulerDeclarations.h>

class ModuleTemplateClass
{
public:
    ModuleTemplateClass();
    void init(Scheduler& scheduler);


private:
    void loop();
    Task _loopTask;

	bool NewFunction(bool);

	unsigned int Counter;

};

extern ModuleTemplateClass ModuleTemplate;
