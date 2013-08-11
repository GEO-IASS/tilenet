#pragma once
#ifndef _TILEY_BLITABLE_HPP
#define _TILEY_BLITABLE_HPP



#include "lockable.hpp"
#include "flushable.hpp"

namespace tiley {


template<typename E>
class Context;


template<typename Elem>
class DefaultValuable
{
public:
	typedef Elem value_type;

public:
	DefaultValuable(value_type* defVal = nullptr)
		: mDefaultValue(defVal)
	{
	}

	const value_type& defaultValue() const
	{
		assert(mDefaultValue);
		return *mDefaultValue;
	}

	void defaultValue(const value_type* defV )
	{
		mDefaultValue = defV;
	}

private:
	value_type* mDefaultValue;
};

template<typename E>
class Blitable
	: public Lockable
	, public virtual Flushable
{
	template<typename E>
	friend class Context;
public:
	typedef E value_type;
	typedef Rect size_type;
public:
	inline virtual ~Blitable(){}

	virtual const size_type& size() const;

protected:
	virtual const value_type& get(const Point& pos) const = 0;
	virtual void set(const value_type& e, const Point& pos) = 0;

};



template<typename E>
class Context
	: public Lock
	, public AutoFlusher
	, public DefaultValuable<E>
{
public:
	typedef E value_type;
	typedef Blitable<value_type> target_type;
	typedef typename target_type::size_type size_type;
public:
	Context(target_type& target, bool flushAtEnd = true)
		: Lock(target)
		, AutoFlusher(flushAtEnd? &target : nullptr)
		, mSize(target.size())
	{
	}

	Context(target_type& target, value_type* defVal, bool flushAtEnd = true)
		: Lock(target)
		, AutoFlusher(flushAtEnd? &target : nullptr)
		, DefaultValuable(defVal)
		, mSize(target.size())
	{
	}

	inline const value_type& get(const Point& pos) const
	{
		if(pos.isIn(mSize))
			return mTarget.get(mPos + pos);
		else
			return defaultValue();
	}

	inline void set(const value_type& e, const Point& pos)
	{
		if(pos.isIn(mSize))
			mTarget.set(e, mPos + pos);
	}
	

private:
	target_type& mTarget;
	size_type mSize;
};



}



#endif