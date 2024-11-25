import("stdfaust.lib");
release = hslider("rel",1,1,10,0.01);
timbre(a) = os.osc(a)*0.5 + os.osc(a*2)*0.25 + os.osc(a*3)*0.125;
timbre2(b) = os.osc(b)*0.5 + os.osc(b*2)*0.25 + os.osc(b*3)*0.125;
timbre3(c) = os.osc(c)*0.5 + os.osc(c*2)*0.25 + os.osc(c*3)*0.125;
timbre4(d) = os.osc(d)*0.5 + os.osc(d*2)*0.25 + os.osc(d*3)*0.125;
timbre5(e) = os.osc(e)*0.5 + os.osc(e*2)*0.25 + os.osc(e*3)*0.125;
timbre6(f) = os.osc(f)*0.5 + os.osc(f*2)*0.25 + os.osc(f*3)*0.125;
timbre7(g) = os.osc(g)*0.5 + os.osc(g*2)*0.25 + os.osc(g*3)*0.125;

knap = button("A") ;
knap2 = button("B");
knap3 = button("C");
knap4 = button("D");
knap5 = button("E");
knap6 = button("F");
knap7 = button("G");

process = knap : en.adsr(0.01,2,2,release)*(timbre(220)/3) + (knap2 : en.adsr(0.01,2,2,release)*(timbre2(246.94)/3) + (knap3 : en.adsr(0.01,2,2,release)*(timbre3(261.63)/3)) + (knap4 : en.adsr(0.01,2,2,release)*(timbre4(293.66)/3)) + (knap5 : en.adsr(0.01,2,2,release)*(timbre5(329.63)/3)) + (knap6 : en.adsr(0.01,2,2,release)*(timbre6(349.23)/3)) + (knap7 : en.adsr(0.01,2,2,release)*(timbre7(392.00)/3))) : _ <: _,_;