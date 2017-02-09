#include <echelon/util/spidermonkey-runner.hpp>

#ifdef WITH_SPIDER_MONKEY_RUNNER

using namespace JS;

// The class of the global object.
static JSClass globalClass = {
    "global",
    JSCLASS_GLOBAL_FLAGS,
    JS_PropertyStub,
    JS_DeletePropertyStub,
    JS_PropertyStub,
    JS_StrictPropertyStub,
    JS_EnumerateStub,
    JS_ResolveStub,
    JS_ConvertStub,
    nullptr, nullptr, nullptr, nullptr,
    JS_GlobalObjectTraceHook
};

// The error reporter callback.
void reportError(JSContext *cx, const char *message, JSErrorReport *report) {
  fprintf(stderr, "%s:%u:%s\n",
          report->filename ? report->filename : "[no filename]",
          (unsigned int) report->lineno,
          message);
}

int run(JSContext *cx) {
  // Enter a request before running anything in the context.
  JSAutoRequest ar(cx);

  // Create the global object and a new compartment.
  RootedObject global(cx);
  global = JS_NewGlobalObject(cx, &globalClass, nullptr,
                              JS::DontFireOnNewGlobalHook);
  if (!global)
    return 1;

  // Enter the new global object's compartment.
  JSAutoCompartment ac(cx, global);

  // Populate the global object with the standard globals, like Object and
  // Array.
  if (!JS_InitStandardClasses(cx, global))
    return 1;

  // Your application code here. This may include JSAPI calls to create your
  // own custom JS objects and run scripts.

  return 0;
}

int runner() {
  // Initialize the JS engine.
  if (!JS_Init())
    return 1;

  // Create a JS runtime.
  JSRuntime *rt = JS_NewRuntime(8L * 1024L * 1024L);
  if (!rt)
    return 1;

  // Create a context.
  JSContext *cx = JS_NewContext(rt, 8192);
  if (!cx)
    return 1;
  JS_SetErrorReporter(rt, reportError);

  int status = run(cx);

  // Shut everything down.
  JS_DestroyContext(cx);
  JS_DestroyRuntime(rt);
  JS_ShutDown();

  return status;
}

#endif