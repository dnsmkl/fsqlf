
UPDATE s1.t1
SET
  c1 = x
WHERE 1 = 1
;




UPDATE at1
FROM t1 at1
, t2 at2
SET
  c1 = x
, c2 = y
WHERE at1.id = at2.id
;
