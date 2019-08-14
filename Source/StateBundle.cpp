/*
  ==============================================================================

    StateBundle.cpp
    Created: 14 Aug 2019 12:54:29pm
    Author:  artyom

  ==============================================================================
*/

#include "StateBundle.h"

StateBundle::StateBundle(ValueTree &stateRoot) : stateRoot(stateRoot)
{

}

void StateBundle::addProperty(var property, const String &name)
{
    ValueTree valueTree = ValueTree(Identifier(name));
    stateRoot.appendChild(valueTree, nullptr);
    stateRoot.setProperty(name, property, nullptr);
}

const var& StateBundle::getProperty(const String &name)
{
    return stateRoot.getProperty(name);
}

bool StateBundle::hasProperty(const String &name)
{
    return stateRoot.hasProperty(name);
}
