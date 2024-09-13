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

// A dummy options.h file that is free from Geeqie's dependencies and
// has hardcoded option values.

static struct options_t {
    bool rot_invariant_sim;
    struct {
        bool enabled;
        bool grayscale;
    } alternate_similarity_algorithm;
} options_ = {
    .rot_invariant_sim = true,
    .alternate_similarity_algorithm = {
        .enabled = false,
        .grayscale = false,
    }
};
static struct options_t *options = &options_;
