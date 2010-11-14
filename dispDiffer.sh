#Turns example.cl into out.s

#add -r as an argument to run it as well.

rm disp_us.s
rm disp_them.s
python disp_differ.py us.s > disp_us.s
python disp_differ.py them.s > disp_them.s
diff disp_us.s disp_them.s



