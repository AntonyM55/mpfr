/* tfprintf.c -- test file for mpfr_fprintf and mpfr_vfprintf

Copyright 2008 Free Software Foundation, Inc.
Contributed by the Arenaire and Cacao projects, INRIA.

The GNU MPFR Library is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 2.1 of the License, or (at your
option) any later version.

The GNU MPFR Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the GNU MPFR Library; see the file COPYING.LIB.  If not, write to
the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,
MA 02110-1301, USA. */

#ifdef HAVE_STDARG
#include <stdarg.h>

#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <stddef.h>

#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif

#ifdef HAVE_QUAD_T
#include <sys/types.h>
#endif

#include "mpfr-test.h"

#if MPFR_VERSION >= MPFR_VERSION_NUM(2,4,0)

#define QUOTE(X) NAME(X)
#define NAME(X) #X

#define check_length(num_test, var, value, var_spec)                    \
  if ((var) != (value))                                                 \
    {                                                                   \
      printf ("Error in test #%d: mpfr_vfprintf printed %"QUOTE(var_spec) \
              " characters instead of %d\n", (num_test), (var), (value)); \
      exit (1);                                                         \
    }

#define check_length_with_cmp(num_test, var, value, cmp, var_spec)      \
  if (cmp != 0)                                                         \
    {                                                                   \
      mpfr_printf ("Error in test #%d, mpfr_vfprintf printed %"         \
                   QUOTE(var_spec)" characters instead of %d\n",        \
                   (num_test), (var), (value));                         \
      exit (1);                                                         \
    }

/* limit for random precision in random() */
const int prec_max_printf = 5000;

static void
check (FILE *fout, char *fmt, mpfr_t x)
{
  if (mpfr_fprintf (fout, fmt, x) == -1)
    {
      mpfr_printf ("Error in mpfr_fprintf(fout, \"%s\", %Re)\n",
                   fmt, x);
      exit (1);
    }
  fputc ('\n', fout);
}

static void
check_vfprintf (FILE *fout, char *fmt, ...)
{
  va_list ap;

  va_start (ap, fmt);
  if (mpfr_vfprintf (fout, fmt, ap) == -1)
    {
      mpfr_printf ("Error in mpfr_vfprintf(fout, \"%s\", ...)\n", fmt);

      va_end (ap);
      exit (1);
    }

  va_end (ap);
  fputc ('\n', fout);
}

static void
check_special (FILE *fout)
{
  mpfr_t x;

  mpfr_init (x);

  mpfr_set_inf (x, 1);
  check (fout, "%Ra", x);
  check (fout, "%Rb", x);
  check (fout, "%Re", x);
  check (fout, "%Rf", x);
  check (fout, "%Rg", x);
  check_vfprintf (fout, "%Ra", x);
  check_vfprintf (fout, "%Rb", x);
  check_vfprintf (fout, "%Re", x);
  check_vfprintf (fout, "%Rf", x);
  check_vfprintf (fout, "%Rg", x);

  mpfr_set_inf (x, -1);
  check (fout, "%Ra", x);
  check (fout, "%Rb", x);
  check (fout, "%Re", x);
  check (fout, "%Rf", x);
  check (fout, "%Rg", x);
  check_vfprintf (fout, "%Ra", x);
  check_vfprintf (fout, "%Rb", x);
  check_vfprintf (fout, "%Re", x);
  check_vfprintf (fout, "%Rf", x);
  check_vfprintf (fout, "%Rg", x);

  mpfr_set_nan (x);
  check (fout, "%Ra", x);
  check (fout, "%Rb", x);
  check (fout, "%Re", x);
  check (fout, "%Rf", x);
  check (fout, "%Rg", x);
  check_vfprintf (fout, "%Ra", x);
  check_vfprintf (fout, "%Rb", x);
  check_vfprintf (fout, "%Re", x);
  check_vfprintf (fout, "%Rf", x);
  check_vfprintf (fout, "%Rg", x);

  mpfr_clear (x);
}

static void
check_mixed (FILE *fout)
{
  char ch = 'a';
  signed char sch = -1;
  unsigned char uch = 1;
  short sh = -1;
  unsigned short ush = 1;
  int i = -1;
  unsigned int ui = 1;
  long lo = -1;
  unsigned long ulo = 1;
  float f = -1.25;
  double d = -1.25;
  long double ld = -1.25;

  ptrdiff_t p = 1;
  size_t sz = 1;

  mpz_t mpz;
  mpq_t mpq;
  mpf_t mpf;
  mp_rnd_t rnd = GMP_RNDN;

  mp_size_t limb_size = 3;
  mp_limb_t limb[3];

  mpfr_t mpfr;
  mpfr_prec_t prec = 53;

  mpz_init (mpz);
  mpz_set_ui (mpz, ulo);
  mpq_init (mpq);
  mpq_set_si (mpq, lo, ulo);
  mpf_init (mpf);
  mpf_set_q (mpf, mpq);

  mpfr_init2 (mpfr, prec);
  mpfr_set_f (mpfr, mpf, GMP_RNDN);

  limb[0] = limb[1] = limb[2] = ~ (mp_limb_t) 0;

  check_vfprintf (fout, "a. %Ra, b. %hhu, c. %u, d. %lx%hhn", mpfr, uch, ui,
                  ulo, &uch);
  check_length (1, uch, 28, hhu);
  check_vfprintf (fout, "a. %hhi, b. %Rb, c. %u, d. %li%ln", sch, mpfr, i,
                  lo, &ulo);
  check_length (2, ulo, 37, lu);
  check_vfprintf (fout, "a. %hi, b. %*f, c. %Re%hn", ush, 3, f, mpfr, &ush);
  check_length (3, ush, 29, hu);
  check_vfprintf (fout, "a. %hi, b. %e, c. %#.2Rf%n", sh, d, mpfr, &i);
  check_length (4, i, 33, d);
  check_vfprintf (fout, "a. %R*A, b. %Fe, c. %i%zn", rnd, mpfr, mpf, sz,
                  &sz);
  check_length (5, sz, 34, zu);
  check_vfprintf (fout, "a. %Pu, b. %c, c. %Lf, d. %Zi%Zn", prec, ch, ld,
                  mpz, &mpz);
  check_length_with_cmp (6, mpz, 31, mpz_cmp_ui (mpz, 31), Zi);
  check_vfprintf (fout, "%% a. %#.0RNg, b. %Qx%Rn, c. %td, d. %p", mpfr, mpq,
                  &mpfr, p, &i);
  check_length_with_cmp (7, mpfr, 16, mpfr_cmp_ui (mpfr, 16), Rg);

  check_vfprintf (fout, "a. %Mx b. %Re%Mn", limb[0], mpfr, &limb[0]);
  if (limb[0] != 14 + BITS_PER_MP_LIMB / 4 || limb[1] != ~ (mp_limb_t) 0
      || limb[2] != ~ (mp_limb_t) 0)
    {
      printf ("Error in test #8: mpfr_vfprintf did not print %u characters"
              " as expected\n", 14 + BITS_PER_MP_LIMB / 4);
      exit (1);
    }

  limb[0] = ~ (mp_limb_t) 0;
  /* we tell vfprintf that limb array is 2 cells wide
     and check it doesn't go through */
  check_vfprintf (fout, "a. %Re .b %Nx%Nn", mpfr, limb, limb_size, limb,
                  limb_size - 1);
  if (limb[0] != 14 + 3 * BITS_PER_MP_LIMB / 4 || limb[1] != (mp_limb_t) 0
      || limb[2] != ~ (mp_limb_t) 0)
    {
      printf ("Error in test #9: mpfr_vfprintf did not print %u characters"
              " as expected\n", 14 + BITS_PER_MP_LIMB / 4);
      exit (1);
    }

#ifdef HAVE_LONG_LONG
  {
    long long llo = -1;
    unsigned long long ullo = 1;

    check_vfprintf (fout, "a. %Re, b. %llx%Qn", mpfr, ullo, &mpq);
    check_length_with_cmp (11, mpq, 16, mpq_cmp_ui (mpq, 16, 1), Qu);
    check_vfprintf (fout, "a. %lli, b. %Rf%Fn", llo, mpfr, &mpf);
    check_length_with_cmp (12, mpf, 12, mpf_cmp_ui (mpf, 12), Fg);
  }
#endif

#ifdef HAVE_QUAD_T
  {
    quad_t q = -1;
    u_quad_t uq = 1;

    check_vfprintf (fout, "a. %Re, b. %qx%Qn", mpfr, uq, &mpq);
    check_length_with_cmp (21, mpq, 16, mpq_cmp_ui (mpq, 16, 1), Qu);
    check_vfprintf (fout, "a. %qi, b. %Rf%Fn", q, mpfr, &mpf);
    check_length_with_cmp (22, mpf, 12, mpf_cmp_ui (mpf, 12), Fg);
  }
#endif

#ifdef _MPFR_H_HAVE_INTMAX_T
  {
    intmax_t im = -1;
    uintmax_t uim = 1;

    check_vfprintf (fout, "a. %*RA, b. %ji%Qn", 10, mpfr, im, &mpq);
    check_length_with_cmp (31, mpq, 20, mpq_cmp_ui (mpq, 20, 1), Qu);
    check_vfprintf (fout, "a. %.*Re, b. %jx%Fn", 10, mpfr, uim, &mpf);
    check_length_with_cmp (32, mpf, 25, mpf_cmp_ui (mpf, 25), Fg);
  }
#endif

  mpfr_clear (mpfr);
  mpf_clear (mpf);
  mpq_clear (mpq);
  mpz_clear (mpz);
}

static void
check_random (FILE *fout, int nb_tests)
{
  int i;
  mpfr_t x;
  mp_rnd_t rnd;
  char flag[] =
    {
      '-',
      '+',
      ' ',
      '#',
      '0', /* no ambiguity: first zeros are flag zero*/
      '\''
    };
  char specifier[] =
    {
      'a',
      'b',
      'e',
      'f',
      'g'
    };
  mp_exp_t old_emin, old_emax;

  old_emin = mpfr_get_emin ();
  old_emax = mpfr_get_emax ();

  mpfr_init (x);

  for (i = 0; i < nb_tests; ++i)
    {
      int ret;
      int j, jmax;
      int spec, prec;
#define FMT_SIZE 13
      char fmt[FMT_SIZE]; /* at most something like "%-+ #0'.*R*f" */
      char *ptr = fmt;

      tests_default_random (x, 256, MPFR_EMIN_MIN, MPFR_EMAX_MAX);
      rnd = RND_RAND ();

      spec = (int) (randlimb () % 5);
      jmax = (spec == 3 || spec == 4) ? 6 : 5; /* ' flag only with %f or %g */
      /* advantage small precision */
      prec = (int) (randlimb () % ((randlimb () % 2) ? 10 : prec_max_printf));
      if (spec == 3 && mpfr_get_exp (x) > prec_max_printf)
        /*  change style 'f' to style 'e' when number x is large */
        --spec;

      *ptr++ = '%';
      for (j = 0; j < jmax; j++)
        {
          if (randlimb () % 3 == 0)
            *ptr++ = flag[j];
        }
      *ptr++ = '.';
      *ptr++ = '*';
      *ptr++ = 'R';
      *ptr++ = '*';
      *ptr++ = specifier[spec];
      *ptr = '\0';
      MPFR_ASSERTD (ptr - fmt < FMT_SIZE);

      mpfr_fprintf (fout, "mpfr_fprintf(fout, \"%s\", %d, %s, %Re)\n",
                    fmt, prec, mpfr_print_rnd_mode (rnd), x);
      ret = mpfr_fprintf (fout, fmt, prec, rnd, x);
      if (ret == -1)
        {
          if (spec == 3
              && (MPFR_GET_EXP (x) > INT_MAX || MPFR_GET_EXP (x) < -INT_MAX))
            /* normal failure: x is too large to be output with full precision */
            {
              mpfr_fprintf (fout, "too large !");
            }
          else
            {
              mpfr_printf ("Error in mpfr_fprintf(fout, \"%s\", %d, %s, %Re)\n",
                           fmt, prec, mpfr_print_rnd_mode (rnd), x);
              exit (1);
            }
        }
      mpfr_fprintf (fout, "\n");
    }

  mpfr_set_emin (old_emin);
  mpfr_set_emax (old_emax);

  mpfr_clear (x);
}

int
main (int argc, char *argv[])
{
  FILE *fout;
  int N;

  tests_start_mpfr ();

  /* with no argument: prints to /dev/null,
     tfprintf N: prints N tests to stdout */
  if (argc == 1)
    {
      N = 1000;
      fout = fopen ("/dev/null", "w");
      /* If we failed to open this device, try with a dummy file */
      if (fout == NULL)
        {
          fout = fopen ("mpfrtest.txt", "w");

          if (fout == NULL)
            {
              printf ("Can't open /dev/null or a temporary file\n");
              exit (1);
            }
        }
    }
  else
    {
      fout = stdout;
      N = atoi (argv[1]);
    }

  check_special (fout);
  check_mixed (fout);
  check_random (fout, N);

  fclose (fout);
  tests_end_mpfr ();
  return 0;
}

#else  /* MPFR_VERSION */

int
main (void)
{
  printf ("Warning! Test disabled for this MPFR version.\n");
  return 0;
}

#endif  /* MPFR_VERSION */

#else  /* HAVE_STDARG */

int
main (void)
{
  /* We have nothing to test. */
  return 0;
}

#endif  /* HAVE_STDARG */
