BEGIN {
	printf("This awk file is for zdpa!\n");
	success_pkts=0;
	sent_pkts=0;
}
{

 	action = $1;
    	time = $2;
    	src_node = $3;
    	dst_node = $4;
    	pkt_type = $5;
    	pktsize = $6;
    	flow_id = $8;
    	src_node_port = $9;
    	dst_node_port = $10;
    	seq_no = $11;
    	pkt_id = $12;

	if(action=="-"  ){
		if(a[pkt_id]==0){
			offered_pkt++;
		}
                a[pkt_id]++;
		sent_pkts++;	
	}

    	if(action=="r" ){
    		success_pkts ++;
    	}
	
 
  	st=9;
}
END{	printf("\n")>>"/home/hadoop/chenj/ns-allinone-2.35/ns-2.35/record.txt";
	printf("offered_pkt = %.4f\t",offered_pkt)>>"/home/hadoop/chenj/ns-allinone-2.35/ns-2.35/record.txt";
        printf("offered load =  %.4f\t\t",offered_pkt*21*8/st/10000)>>"/home/hadoop/chenj/ns-allinone-2.35/ns-2.35/record.txt";
	printf("sent_pkts =  %.4f\t",sent_pkts)>>"/home/hadoop/chenj/ns-allinone-2.35/ns-2.35/record.txt";
	printf("traffic load =  %.4f\t\t",sent_pkts*21*8/st/10000)>>"/home/hadoop/chenj/ns-allinone-2.35/ns-2.35/record.txt";
	
	printf("success_pkts =  %.4f\t",success_pkts)>>"/home/hadoop/chenj/ns-allinone-2.35/ns-2.35/record.txt";
	printf("throughput=  %.4f\t\t",success_pkts*21*8/st/10000)>>"/home/hadoop/chenj/ns-allinone-2.35/ns-2.35/record.txt";
		
	printf("\n\n")>>"/home/hadoop/chenj/ns-allinone-2.35/ns-2.35/record.txt";	
}
