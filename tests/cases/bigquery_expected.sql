
SELECT
  asbaba AS g
, cast ( bfbfb AS date ) + 1 as x
, 2 - 1 as b1
, 2 * 1 as c1
, 2 / 1 as d1
, "str" || "cat"as concatted /*/*/
, min ( a ) over ( partition by x,c order by r rows between 1 preceeding and 1 preceeding )
, 'dfgdf/*gdfgd*/fg''dfgdf/*gdfgd*/fg''dfgdf/*gdfgd*/fg' as c /*a*a*a*/
, cast ( 19.2 AS decimal ( 18,2 ) ) /******/
, ' */ ' /**/
, cast ( 19.2 AS decimal ( 18,2 ) ) /* sdsd  *****---*/
, asextract ( day FROM t ) as _toto -- comment case select from
FROM ( bobo as gogo
LEFT JOIN
(
    SELECT
      1
    FROM
    (
        SELECT
          1
        FROM T

            UNION
        SELECT
          2

            INTERSECT
        SELECT
          2

            EXCEPT
        SELECT
          2
    ) T
) baba
 ON 1 IN ( 1,2,3 )
OR x <> 1
AND 1 = 1
AND 1 = 1
AND "a" LIKE "b")
LEFT JOIN ( baba
CROSS JOIN gaga )
 ON baba.g = T.z
WHERE toto
AND EXISTS
(
    SELECT
      1
    FROM t
    WHERE b = g
    AND NOT EXISTS
    (
        SELECT
          1
        FROM T
    )
)
AND 1 = 1
AND 1 < 1
AND 1 > 1
AND 1 >= 1
AND 1 <= 1
AND 1 <> 1
AND a IN
(
    SELECT
      1
    FROM hsd
)
AND b IN ( 19,2,3 )
;




--
