/*
 *
 * @file		e4c_rsc.c
 *
 * exceptions4c automatic resource handling source code file
 *
 * @version		2.0
 * @author		Copyright (c) 2012 Guillermo Calvo
 *
 * This is free software: you can redistribute it and/or modify it under the
 * terms of the **GNU Lesser General Public License** as published by the
 * *Free Software Foundation*, either version 3 of the License, or (at your
 * option) any later version.
 *
 * This software is distributed in the hope that it will be useful, but
 * **WITHOUT ANY WARRANTY**; without even the implied warranty of
 * **MERCHANTABILITY** or **FITNESS FOR A PARTICULAR PURPOSE**. See the
 * [GNU Lesser General Public License](http://www.gnu.org/licenses/lgpl.html)
 * for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software. If not, see <http://www.gnu.org/licenses/>.
 *
 */

/*
 * this file is undocumented on purpose (everything is documented in the header)
 */


# include "e4c_rsc.h"


E4C_DEFINE_EXCEPTION(MemoryAllocationException,			"Couldn't allocate memory.",		NotEnoughMemoryException);
E4C_DEFINE_EXCEPTION(FileException,						"File exception.",					InputOutputException);
E4C_DEFINE_EXCEPTION(FileOpenException,					"Couldn't open file.",				FileException);
E4C_DEFINE_EXCEPTION(FileCloseException,				"Couldn't close file.",				FileException);
E4C_DEFINE_EXCEPTION(MutexException,					"Mutex error.",						RuntimeException);
E4C_DEFINE_EXCEPTION(MutexLockException,				"Couldn't lock mutex.",				MutexException);
E4C_DEFINE_EXCEPTION(MutexUnlockException,				"Couldn't unlock mutex.",			MutexException);
