#pragma once

#include "V8Helpers.h"

/**
 * Needed cause MSVC requires globals to be references
 * or they will be optimized away in static builds
 * https://stackoverflow.com/questions/9459980/c-global-variable-not-initialized-when-linked-through-static-libraries-but-ok
 * the other solutions didn't work
 *
 * Once the module is completely a DLL, this will not be necessary
 */
inline void RegisterEvents()
{
    // Shared
    V8_REFERENCE_LOCAL_EVENT_HANDLER(consoleCommand);
    V8_REFERENCE_LOCAL_EVENT_HANDLER(animationChange);
    V8_REFERENCE_LOCAL_EVENT_HANDLER(baseObjectCreate);
    V8_REFERENCE_LOCAL_EVENT_HANDLER(baseObjectRemove);

    // Main
    V8_REFERENCE_EVENT_HANDLER(clientScriptEvent);
    V8_REFERENCE_EVENT_HANDLER(serverScriptEvent);
    V8_REFERENCE_EVENT_HANDLER(webviewEvent);
    V8_REFERENCE_EVENT_HANDLER(webSocketEvent);
    V8_REFERENCE_EVENT_HANDLER(audioEvent);
    V8_REFERENCE_EVENT_HANDLER(keyboardEvent);
    V8_REFERENCE_LOCAL_EVENT_HANDLER(connectionComplete);
    V8_REFERENCE_LOCAL_EVENT_HANDLER(disconnect);

    // Entity
    V8_REFERENCE_EVENT_HANDLER(gameEntityCreate);
    V8_REFERENCE_EVENT_HANDLER(gameEntityDestroy);
    V8_REFERENCE_LOCAL_EVENT_HANDLER(taskChange);
    V8_REFERENCE_LOCAL_EVENT_HANDLER(playerWeaponShoot);
    V8_REFERENCE_LOCAL_EVENT_HANDLER(playerWeaponChange);
    V8_REFERENCE_LOCAL_EVENT_HANDLER(weaponDamage);

    // Meta
    V8_REFERENCE_LOCAL_EVENT_HANDLER(syncedMetaChange);
    V8_REFERENCE_LOCAL_EVENT_HANDLER(streamSyncedMetaChange);
    V8_REFERENCE_LOCAL_EVENT_HANDLER(globalSyncedMetaChange);
    V8_REFERENCE_LOCAL_EVENT_HANDLER(globalMetaChange);

    // Resource
    V8_REFERENCE_LOCAL_EVENT_HANDLER(anyResourceStart);
    V8_REFERENCE_EVENT_HANDLER(anyResourceStop);
    V8_REFERENCE_LOCAL_EVENT_HANDLER(anyResourceError);

    // Vehicle
    V8_REFERENCE_LOCAL_EVENT_HANDLER(enteredVehicle);
    V8_REFERENCE_LOCAL_EVENT_HANDLER(leftVehicle);
    V8_REFERENCE_LOCAL_EVENT_HANDLER(changedVehicleSeat);
}
