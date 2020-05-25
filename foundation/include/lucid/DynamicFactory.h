//
// dynamicFactory.h
//
// Library: Foundation
// Package: Core
// Module:  dynamicFactory
//
// Definition of the dynamicFactory class.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_dynamicFactory_INCLUDED
#define Foundation_dynamicFactory_INCLUDED


#include "lucid/Foundation.h"
#include "lucid/Instantiator.h"
#include "lucid/Exception.h"
#include "lucid/Mutex.h"
#include <map>
#include <memory>


namespace Lucid {


template <class Base>
class dynamicFactory
	/// A factory that creates objects by class name.
{
public:
	typedef AbstractInstantiator<Base> AbstractFactory;

	dynamicFactory()
		/// Creates the dynamicFactory.
	{
	}

	~dynamicFactory()
		/// Destroys the dynamicFactory and deletes the instantiators for
		/// all registered classes.
	{
		for (auto& p: _map)
		{
			delete p.second;
		}
	}

	Base* createInstance(const std::string& className) const
		/// Creates a new instance of the class with the given name.
		/// The class must have been registered with registerClass.
		/// If the class name is unknown, a NotFoundException is thrown.
	{
		FastMutex::ScopedLock lock(_mutex);

		typename FactoryMap::const_iterator it = _map.find(className);
		if (it != _map.end())
			return it->second->createInstance();
		else
			throw NotFoundException(className);
	}

	template <class C>
	void registerClass(const std::string& className)
		/// Registers the instantiator for the given class with the dynamicFactory.
		/// The dynamicFactory takes ownership of the instantiator and deletes
		/// it when it's no longer used.
		/// If the class has already been registered, an ExistsException is thrown
		/// and the instantiator is deleted.
	{
		registerClass(className, new Instantiator<C, Base>);
	}

	void registerClass(const std::string& className, AbstractFactory* pAbstractFactory)
		/// Registers the instantiator for the given class with the dynamicFactory.
		/// The dynamicFactory takes ownership of the instantiator and deletes
		/// it when it's no longer used.
		/// If the class has already been registered, an ExistsException is thrown
		/// and the instantiator is deleted.
	{
		poco_check_ptr (pAbstractFactory);

		FastMutex::ScopedLock lock(_mutex);

		std::unique_ptr<AbstractFactory> ptr(pAbstractFactory);

		typename FactoryMap::iterator it = _map.find(className);
		if (it == _map.end())
			_map[className] = ptr.release();
		else
			throw ExistsException(className);
	}

	void unregisterClass(const std::string& className)
		/// Unregisters the given class and deletes the instantiator
		/// for the class.
		/// Throws a NotFoundException if the class has not been registered.
	{
		FastMutex::ScopedLock lock(_mutex);

		typename FactoryMap::iterator it = _map.find(className);
		if (it != _map.end())
		{
			delete it->second;
			_map.erase(it);
		}
		else throw NotFoundException(className);
	}

	bool isClass(const std::string& className) const
		/// Returns true iff the given class has been registered.
	{
		FastMutex::ScopedLock lock(_mutex);

		return _map.find(className) != _map.end();
	}

private:
	dynamicFactory(const dynamicFactory&);
	dynamicFactory& operator = (const dynamicFactory&);

	typedef std::map<std::string, AbstractFactory*> FactoryMap;

	FactoryMap _map;
	mutable FastMutex _mutex;
};


} // namespace Lucid


#endif // Foundation_dynamicFactory_INCLUDED
