# $OpenBSD: Makefile,v 1.19 2023/11/15 00:52:42 millert Exp $

REGRESS_TARGETS=t1 t2 t3 t4 t5 t6 t7 t8 t9 t10 t11 t12 t13 t14 t15 t16 t17 \
	t18 t19 t20 t21 t22 t23 t24 t25 t26 t27 t28

t1:
	grep t.s ${.CURDIR}/in | diff - ${.CURDIR}/t1.out

t2:
	grep -w separated ${.CURDIR}/in | diff - ${.CURDIR}/t1.out

t3:
	grep ^Beginning ${.CURDIR}/in | diff - ${.CURDIR}/t3.out

t4:
	grep -i end$$ ${.CURDIR}/in | diff - ${.CURDIR}/t4.out

t5:
	egrep '\?|\*$$' ${.CURDIR}/in | diff - ${.CURDIR}/t5.out

t6:
	grep -v [l] ${.CURDIR}/in | diff - ${.CURDIR}/t6.out

t7:
	grep -x line ${.CURDIR}/in | diff - ${.CURDIR}/t7.out

t8:
	fgrep line. ${.CURDIR}/in | diff - /dev/null

t9:
	grep non.existent ${.CURDIR}/in | diff - /dev/null

t10:
	grep -s foo nonexistentfile | diff - /dev/null

t11:
	grep -C1 examples ${.CURDIR}/in | diff - ${.CURDIR}/t11.out

t12:
	egrep -f ${.CURDIR}/t1.out ${.CURDIR}/in | diff - ${.CURDIR}/t1.out

t13:
	grep 'abcd. abcd.' ${.CURDIR}/t13.in | diff - ${.CURDIR}/t13.out

t14:
	test X`grep -w -l u_int ${.CURDIR}/t14.in` = X${.CURDIR}/t14.in

t15:
	grep -w tu ${.CURDIR}/t15.in | diff - ${.CURDIR}/t15.out

t16:
	grep -w to ${.CURDIR}/t15.in | diff - ${.CURDIR}/t15.out

t17:
	grep -w '^foo' ${.CURDIR}/t17.in | diff - ${.CURDIR}/t17.out

t18:
	grep -w '^foo$$' ${.CURDIR}/t17.in | diff - ${.CURDIR}/t17.out

t19:
	grep  -w 'a.' ${.CURDIR}/t19.in | diff - ${.CURDIR}/t19.out
	fgrep -w -e 'aa' -e 'a_' ${.CURDIR}/t19.in | diff - ${.CURDIR}/t19.out

t20:
	echo '='   | grep  -w  '='  | diff - /dev/null
	echo '='   | fgrep -w  '='  | diff - /dev/null
	echo 'a='  | grep  -w  'a=' | diff - /dev/null
	echo 'a='  | fgrep -w  'a=' | diff - /dev/null
	echo '=b'  | grep  -w  '=b' | diff - /dev/null
	echo '=b'  | fgrep -w  '=b' | diff - /dev/null
	echo 'a=b' | grep  -qw 'a=b'
	echo 'a=b' | fgrep -qw 'a=b'
	echo '+'   | grep  -w  '+'  | diff - /dev/null
	echo '+'   | fgrep -w  '+'  | diff - /dev/null
	echo 'a+'  | grep  -w  'a+' | diff - /dev/null
	echo 'a+'  | fgrep -w  'a+' | diff - /dev/null
	echo '+b'  | grep  -w  '+b' | diff - /dev/null
	echo '+b'  | fgrep -w  '+b' | diff - /dev/null
	echo 'a+b' | grep  -qw 'a+b'
	echo 'a+b' | fgrep -qw 'a+b'

t21:
	grep -l D.e ${.CURDIR}/in | diff - ${.CURDIR}/t21.out
	egrep -l D.e ${.CURDIR}/in | diff - ${.CURDIR}/t21.out
	grep -l '.*D.e' ${.CURDIR}/in | diff - ${.CURDIR}/t21.out
	egrep -l '.*D.e' ${.CURDIR}/in | diff - ${.CURDIR}/t21.out

t22:
	grep -l 'a.*b' ${.CURDIR}/t22.in

t23:
	egrep -w 'word1|word2|word3' ${.CURDIR}/t23.in

t24:
	grep -e '' < ${.CURDIR}/in | diff - ${.CURDIR}/in
	grep -x -e '' < ${.CURDIR}/in | diff - /dev/null
	grep -f /dev/null < ${.CURDIR}/in | diff - /dev/null

t25:
	grep -o [ab] ${.CURDIR}/t25.in | diff - ${.CURDIR}/t25.out

t26:
	echo 'aaab' | grep -o 'a*' | head -n 10 | diff - ${.CURDIR}/t26.out

t27:
	grep -A1 'C' ${.CURDIR}/t27.in | diff - ${.CURDIR}/t27a.out
	grep -B1 'C' ${.CURDIR}/t27.in | diff - ${.CURDIR}/t27b.out
	grep -C1 'C' ${.CURDIR}/t27.in | diff - ${.CURDIR}/t27c.out

t28:
	grep -m 0 -o x.y ${.CURDIR}/t28.in | diff - /dev/null
	grep -m 1 -o x.y ${.CURDIR}/t28.in | diff - ${.CURDIR}/t28_1.out
	grep -m 2 -o x.y ${.CURDIR}/t28.in | diff - ${.CURDIR}/t28_2.out
	grep -m 3 -o x.y ${.CURDIR}/t28.in | diff - ${.CURDIR}/t28_3.out
	grep -m 4 -o x.y ${.CURDIR}/t28.in | diff - ${.CURDIR}/t28_4.out
	grep -m 5 -o x.y ${.CURDIR}/t28.in | diff - ${.CURDIR}/t28_4.out
	grep -m 6 -o x.y ${.CURDIR}/t28.in | diff - ${.CURDIR}/t28_4.out
	grep -m 7 -o x.y ${.CURDIR}/t28.in | diff - ${.CURDIR}/t28_4.out
	grep -m 8 -o x.y ${.CURDIR}/t28.in | diff - ${.CURDIR}/t28_4.out
	grep -m 0 x.y ${.CURDIR}/t28.in | diff - /dev/null
	grep -m 1 x.y ${.CURDIR}/t28.in | diff - ${.CURDIR}/t28_5.out
	grep -m 2 x.y ${.CURDIR}/t28.in | diff - ${.CURDIR}/t28_6.out
	grep -m 3 x.y ${.CURDIR}/t28.in | diff - ${.CURDIR}/t28_7.out
	grep -m 4 x.y ${.CURDIR}/t28.in | diff - ${.CURDIR}/t28_8.out
	grep -m 5 x.y ${.CURDIR}/t28.in | diff - ${.CURDIR}/t28_8.out
	grep -m 6 x.y ${.CURDIR}/t28.in | diff - ${.CURDIR}/t28_8.out
	grep -m 7 x.y ${.CURDIR}/t28.in | diff - ${.CURDIR}/t28_8.out
	grep -m 8 x.y ${.CURDIR}/t28.in | diff - ${.CURDIR}/t28_8.out

.PHONY: t1 t2 t3 t4 t5 t6 t7 t8 t9 t10 t11 t12 t13 t14 t15 t16 t17 t18 t19 t20
.PHONY: t21 t22 t23 t24 t25 t26 t27 t28

.include <bsd.regress.mk>
