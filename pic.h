/*
 * image-duplicate-finder - Command-line tool for finding image duplicates
 * Copyright (C) 2024 Marcin Owsiany <marcin@owsiany.pl>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <set>
#include <string>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <glib/gtypes.h>

#include "similar.h"

class pic {
public:
    pic(char const *cname);
    ~pic();
    gdouble compare(const pic&);
    std::string name;
    std::set<std::string> equivalent;
private:
    ImageSimilarityData *sim;
friend int operator<(const pic &a, const pic &b);
};
