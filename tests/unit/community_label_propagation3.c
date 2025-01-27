/* -*- mode: C -*-  */
/* vim:set ts=4 sw=4 sts=4 et: */
/*
   IGraph library.
   Copyright (C) 2007-2012  Gabor Csardi <csardi.gabor@gmail.com>
   334 Harvard street, Cambridge, MA 02139 USA

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc.,  51 Franklin Street, Fifth Floor, Boston, MA
   02110-1301 USA

*/

#include <igraph.h>

#include "test_utilities.inc"

int main() {
    igraph_t g;
    igraph_vector_t membership, initial;
    igraph_vector_bool_t fixed;
    long int i;

    /* label propagation is a stochastic method */
    igraph_rng_seed(igraph_rng_default(), 765);

    /* Zachary Karate club -- does not matter, we are simply interested in
     * whether the system handles it correctly if a fixed node is unlabeled */
    igraph_famous(&g, "zachary");


    igraph_vector_init(&initial, igraph_vcount(&g));
    igraph_vector_fill(&initial, -1);
    igraph_vector_bool_init(&fixed, igraph_vcount(&g));
    igraph_vector_bool_fill(&fixed, 0);
    VECTOR(fixed)[7] = 1;
    VECTOR(fixed)[13] = 1;

    igraph_vector_init(&membership, 0);

    igraph_community_label_propagation(&g, &membership, 0, &initial, &fixed,
                                       /*modularity=*/ 0);

    for (i = 0; i < igraph_vcount(&g); i++) {
        /* Check that the "fixed" vector has not been changed */
        if (i == 7 || i == 13) {
            if (!VECTOR(fixed)[i]) {
                return 1;
            }
        } else {
            if (VECTOR(fixed)[i]) {
                return 1;
            }
        }

        /* Check that no vertex remained unlabeled */
        if (VECTOR(membership)[i] < 0) {
            return 2;
        }
    }

    igraph_vector_bool_destroy(&fixed);
    igraph_vector_destroy(&initial);
    igraph_vector_destroy(&membership);

    igraph_destroy(&g);

    VERIFY_FINALLY_STACK();

    return 0;
}
