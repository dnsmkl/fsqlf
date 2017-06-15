select
case when case X when 1 then 'lv2:1' else 'lv2:not1' end = 1 and x = x
then 'LV1'
else 'still-LV1'
end
