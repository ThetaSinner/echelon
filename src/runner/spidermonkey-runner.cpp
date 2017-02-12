#include <echelon/runner/spidermonkey-runner.hpp>

#define __STDC_LIMIT_MACROS
#include <stdint.h>

#ifdef WITH_SPIDER_MONKEY_RUNNER

/* The class of the global object. */
static JSClass global_class = {
  "global",
  JSCLASS_GLOBAL_FLAGS,
  nullptr,
  nullptr,
  nullptr,
  nullptr,
  nullptr,
  nullptr,
  nullptr,
  nullptr,
  nullptr,
  nullptr,
  nullptr,
  JS_GlobalObjectTraceHook
};

void runner(RunnerData& runnerData)
{
  if (!JS_Init()) {
    throw std::runtime_error("Failed to initialise.");
  }

  JSRuntime *rt = JS_NewRuntime(8L * 1024 * 1024);
  if (!rt) {
    throw std::runtime_error("Failed to create runtime.");
  }

  JSContext *cx = JS_NewContext(rt, 8192);
  if (!cx) {
    throw std::runtime_error("Failed to create context.");
  }

  { // Scope for our various stack objects (JSAutoRequest, RootedObject), so they all go
    // out of scope before we JS_DestroyContext.

    JSAutoRequest ar(cx); // In practice, you would want to exit this any
    // time you're spinning the event loop

    JS::RootedObject global(cx, JS_NewGlobalObject(cx, &global_class, nullptr, JS::FireOnNewGlobalHook));
    if (!global) {
      throw std::runtime_error("Failed to create global object");
    }

    JS::RootedValue rval(cx);

    { // Scope for JSAutoCompartment
      JSAutoCompartment ac(cx, global);
      JS_InitStandardClasses(cx, global);

      JS::CompileOptions opts(cx);
      opts.setFileAndLine("spider-monkey-input-script", 1);
      bool ok = JS::Evaluate(cx, opts, runnerData.getScript().c_str(), runnerData.getScript().length(), &rval);
      if (!ok) {
        throw std::runtime_error("Failed to execute input script.");
      }
    }

    JSString *str = rval.toString();
    printf("%s\n", JS_EncodeString(cx, str));
  }

  JS_DestroyContext(cx);
  JS_DestroyRuntime(rt);
  JS_ShutDown();
}
#else

void runner(RunnerData& runnerData) {
  throw std::runtime_error("Dummy spidermonkey runner");
}

#endif