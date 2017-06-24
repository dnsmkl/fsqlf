"""Python bindings for libfsqlf."""


from ctypes import CDLL, POINTER, c_char_p, c_int, c_void_p, byref


class Fsqlf(object):
    """Ctypes wrapper around libfsqlf.

    Example:

    >>> with Fsqlf("~/fsqlf/builds/linux-x86/bin/libfsqlf.so") as fsqlf:
    >>>     print(fsqlf.format(" select 4 from t where x in (select 1);"))
    SELECT
      4
    FROM t
    WHERE x IN
    (
        SELECT
          1
    )
    ;
    """
    
    def __init__(self, libpath=None):
        """Initialize formatter.

        Note: Library allocates memory during `__init__`,
        so corresponding `cleanup` call is needed to release the memory.
        (Or use `with` syntax to make cleanup automatically)

        """
        # Specify ctypes stuff.
        self._lib = CDLL(libpath)
        self._lib.fsqlf_kwmap_init.argtypes = [POINTER(c_void_p)]
        self._lib.fsqlf_kwmap_destroy.argtypes = [c_void_p]
        self._lib.fsqlf_format_bytes.argtypes = [c_void_p, c_char_p, c_int, POINTER(c_char_p)]
        
        # Init the keyword configuration.
        self._kwmap = c_void_p()
        self._lib.fsqlf_kwmap_init(byref(self._kwmap))
    
    def format(self, sql_query):
        """Format sql query."""
        result = c_char_p()
        self._lib.fsqlf_format_bytes(self._kwmap, c_char_p(sql_query), len(sql_query), byref(result));
        return result.value

    def cleanup(self):
        """Free-up memory allocated during initialization.

        Formatter is not usable after cleanup.
        """
        self._lib.fsqlf_kwmap_destroy(self._kwmap)

    def __enter__(self):
        return self

    def __exit__(self, type, value, tb):
        self.cleanup()


def format(sql_query, libpath):
    """Format sql query.

    Convenience function that initializes and destroys formatter on each call.
    """
    with Fsqlf(libpath) as fsqlf:
        print(fsqlf.format(sql_query))
