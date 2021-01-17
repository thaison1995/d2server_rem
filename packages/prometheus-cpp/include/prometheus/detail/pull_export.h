
#ifndef PROMETHEUS_CPP_PULL_EXPORT_H
#define PROMETHEUS_CPP_PULL_EXPORT_H

#ifdef PROMETHEUS_CPP_PULL_STATIC_DEFINE
#  define PROMETHEUS_CPP_PULL_EXPORT
#  define PROMETHEUS_CPP_PULL_NO_EXPORT
#else
#  ifndef PROMETHEUS_CPP_PULL_EXPORT
#    ifdef PROMETHEUS_CPP_PULL_EXPORTS
        /* We are building this library */
#      define PROMETHEUS_CPP_PULL_EXPORT 
#    else
        /* We are using this library */
#      define PROMETHEUS_CPP_PULL_EXPORT 
#    endif
#  endif

#  ifndef PROMETHEUS_CPP_PULL_NO_EXPORT
#    define PROMETHEUS_CPP_PULL_NO_EXPORT 
#  endif
#endif

#ifndef PROMETHEUS_CPP_PULL_DEPRECATED
#  define PROMETHEUS_CPP_PULL_DEPRECATED __declspec(deprecated)
#endif

#ifndef PROMETHEUS_CPP_PULL_DEPRECATED_EXPORT
#  define PROMETHEUS_CPP_PULL_DEPRECATED_EXPORT PROMETHEUS_CPP_PULL_EXPORT PROMETHEUS_CPP_PULL_DEPRECATED
#endif

#ifndef PROMETHEUS_CPP_PULL_DEPRECATED_NO_EXPORT
#  define PROMETHEUS_CPP_PULL_DEPRECATED_NO_EXPORT PROMETHEUS_CPP_PULL_NO_EXPORT PROMETHEUS_CPP_PULL_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef PROMETHEUS_CPP_PULL_NO_DEPRECATED
#    define PROMETHEUS_CPP_PULL_NO_DEPRECATED
#  endif
#endif

#endif /* PROMETHEUS_CPP_PULL_EXPORT_H */
