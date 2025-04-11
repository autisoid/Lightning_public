# Coding style

I started writing in Hungarian Notation,
but realize that this is not the most popular style, so I'm willing to compromise.
Try not to deviate too much so the code will look similar across the whole project.

To give examples, these two styles (or anything in between) are fine:

```cpp
template<class _Ty> _Ty FunctionName(_Arg1, _Arg2, ...) {
	if (a == b) {
		DoSomething();
		DoSecondaryStuff();
	} else {
		m_pStuff = nullptr;
		m_nCounter = 0;
	}
	if (m_flX != m_flY)
		PrintStuff();
}

template<class _Ty> _Ty FunctionName(_Arg1, _Arg2, ...)
{
	if(a == b){
		DoSomething();
		DoSecondaryStuff();
	}else{
		m_pStuff = m_nCounter = 0;
	}
	if(m_flX != m_flY)
		PrintStuff();
}
```

This one (or anything more extreme) is heavily discouraged:

```cpp
template< class T >
T FunctionName(arg1, arg2, ...)
{
	if ( a == b )
	{
		DoSomething();
		DoSecondaryStuff();
	}
	else
	{
		m_pStuff = m_nCounter = 0;
	}
	if ( m_flX != m_flY )
	{
		PrintStuff();
	}
}
```

i.e. what's discouraged:

* Putting the brace on the next line as control statements

* Spaces inside control statements

* Putting the brace on the next line after function definitions and structs

* Putting an `else` on the next line as well as the braces

* Put the function return type/template on a separate line

Here are some guidelines how the code should look:

* Use the right types, keywords, prefixes. In particular:

	* try escaping `new` as much as possible, we have `Q_new` for that
	
	* try escaping `malloc` and `free` as much as possible, we have `Q_malloc` and `Q_free` for that respectively
	
	* don't use std, we have functional for that (mostly)

    * don't use types like `__int64`, we have `long long` for that

    * don't use `char` for anything but actual characters, use `bool` for booleans

    * declare pointers like `int* ptr;`, not `int *ptr;`
	
    * most of the variables and stuff employ a form of hungarian notation, i.e.:

    * `m_` is used for class member variables
	
    * `_m_` is used for private class member variables

    * `ms_` for static members
	
	* `g_` for global variables

    * `fl` is a float, `i` or `n` is an integer, `b` is a boolean, `rg` or `a` is an array, `dbl` is a double, `pfn` is a `pointer to function`, `vec` is a `Vector` or `vec3_t`
	
	* we don't use the `class` keyword to define a class, we use `typedef struct` followed by `using` instead:
	
	* `class` is used for `typename` (if not used with a `using` chain)
	
```cpp
typedef struct CMyClass {
	int m_nSomeCounter;
} CMyClass;

using CMyClass = struct CMyClass;
```
	
	* just try looking at globals.cpp/globals.hpp and other files if you're not sure about something 