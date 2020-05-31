#pragma once

#define NODEMODULE_VERSION		u8"node-module v1.0.1"
#define NODEMODULE_COPYRIGHT	u8"Copyright © 2020 altMP team."

#define NODEJS_VERSION			u8"nodejs v12.4.0"
#define NODEJS_COPYRIGHT		u8"Copyright © 2020 Node.js Foundation."

#define NODE_WANT_INTERNALS 1

#define ALT_SERVER_API

#define _USE_MATH_DEFINES

#include <string>
#include <iostream>
#include <functional>
#include <climits>

#include "cpp-sdk/SDK.h"
#include "Log.h"
#include "node.h"
#include "node_platform.h"
#include "node_internals.h"
#include "env.h"
#include "v8.h"
#include "libplatform/libplatform.h"
