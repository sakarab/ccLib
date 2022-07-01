#ifndef SSPORT_H
#define	SSPORT_H

//#if defined(__DLL__)
//	#define	DLL_EXPORT		__declspec(dllexport)
//#else
//	#define	DLL_EXPORT
//#endif

#ifdef	NO_FASTCALL
	#define	FASTCALL
#else
	#define	FASTCALL		__fastcall
#endif

#ifdef	NO_CDFASTCALL
	#define	CDFASTCALL
#else
	#define	CDFASTCALL		__fastcall
#endif

#if defined(S_DEBUG)
	#define	S_DEBUG_MSG(expr)			(expr)
	#define	S_ASSERT(test,expr)			{ if ( test ) (expr) }
#else
	#define	S_DEBUG_MSG(expr)
	#define	S_ASSERT(test,expr)			(expr)
#endif

#define	NO_COPY_CTOR(a)		CDFASTCALL a( const a& src )
#define	NO_COPY_OPER(a)		a& FASTCALL operator=( const a& src )

#endif

