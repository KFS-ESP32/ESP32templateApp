/*
 * Copyright (C) 08/10/2024 Karlfried S.
 */
#include "ModuleTemplate.h"
#include "MessageOutput.h"
#include <Arduino.h>


ModuleTemplateClass ModuleTemplate;

ModuleTemplateClass::ModuleTemplateClass()

    : _loopTask(TASK_IMMEDIATE, TASK_FOREVER, std::bind(&ModuleTemplateClass::loop, this))

{
}



void ModuleTemplateClass::init(Scheduler &scheduler)
{
    Counter = 0;

    scheduler.addTask(_loopTask);
    _loopTask.enable();
}

void ModuleTemplateClass::loop()
{
		NewFunction(false);
}

bool ModuleTemplateClass::NewFunction(bool test)
{
    if (test == true)
    {
	    Counter++;

        if (Counter == 50000)
        {
            MessageOutput.println("Your new function???????????????... ");
            Counter = 0;
        }
    }

	return true;

}

