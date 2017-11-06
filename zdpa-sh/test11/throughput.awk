BEGIN {
	success_pkts=0;
	sent_pkts=0;
	sum1 = 0;
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

			a[pkt_id] = 1;
			sent_pkts ++;
			sum1 ++;

	}

    	if(action=="r" ){
    		success_pkts ++;
		st = time-1;
		sum1 = 0;
    	}
}
END{
	printf("sent_pkts =  %.4f\t",sent_pkts);
	printf("sum1 =  %.4f\t",sum1);		
	sent_pkts -= sum1;
	printf("finish time =  %.4f\t",st);
	printf("sent_pkts =  %.4f\t",sent_pkts);
	printf("offered traffic load =  %.4f\t\t",sent_pkts*21*8/st/10000);
	
	printf("success_pkts =  %.4f\t",success_pkts);
	printf("throughput=  %.4f\t\t",success_pkts*21*8/st/10000);
		
	printf("\n\n");
}
