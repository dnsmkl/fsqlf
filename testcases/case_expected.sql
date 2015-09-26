
SELECT
  CASE
    WHEN CASE X
        WHEN 1
        THEN 'lv2:1'
        ELSE 'lv2:not1'
    END = 1
    THEN 'LV1'
    ELSE 'still-LV1'
END
