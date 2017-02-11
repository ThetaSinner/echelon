#include <echelon/util/spidermonkey-runner.hpp>

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

int runner()
{
  if (!JS_Init()) {
    return 1;
  }

  JSRuntime *rt = JS_NewRuntime(8L * 1024 * 1024);
  if (!rt) {
    return 1;
  }

  JSContext *cx = JS_NewContext(rt, 8192);
  if (!cx) {
    return 1;
  }

  { // Scope for our various stack objects (JSAutoRequest, RootedObject), so they all go
    // out of scope before we JS_DestroyContext.

    JSAutoRequest ar(cx); // In practice, you would want to exit this any
    // time you're spinning the event loop

    JS::RootedObject global(cx, JS_NewGlobalObject(cx, &global_class, nullptr, JS::FireOnNewGlobalHook));
    if (!global)
      return 1;

    JS::RootedValue rval(cx);

    { // Scope for JSAutoCompartment
      JSAutoCompartment ac(cx, global);
      JS_InitStandardClasses(cx, global);

      const char *script = "'hello'+'world, it is '+new Date()";
      const char *filename = "noname";
      int lineno = 1;
      JS::CompileOptions opts(cx);
      opts.setFileAndLine(filename, lineno);
      bool ok = JS::Evaluate(cx, opts, script, strlen(script), &rval);
      if (!ok)
        return 1;
    }

    JSString *str = rval.toString();
    printf("%s\n", JS_EncodeString(cx, str));
  }

  JS_DestroyContext(cx);
  JS_DestroyRuntime(rt);
  JS_ShutDown();
  return 0;
}
#else

int runner() {
  return 1;
}

#endif