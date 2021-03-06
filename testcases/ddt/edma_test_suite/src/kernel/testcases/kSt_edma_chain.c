/*
 * kSt_edma_chain.c
 *
 * This file demonstrates the simple edma mem to mem copy with chaining
 *
 * Copyright (C) 2009 Texas Instruments Incorporated - http://www.ti.com/
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation version 2.
 *
 * This program is distributed <93>as is<94> WITHOUT ANY WARRANTY of any
 * kind, whether express or implied; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

/*Testcode related header files */
#include "kSt_edma.h"


/*
 * Function            		- kSt_edma_memtomemcpytest_chain
 * Functionality      		- This function recieves the EDMA params and performs mem to mem
 *						copy from with two Master DMA channels and chaining mechanism 
 * Input Params   	 	-  acnt,bcnt,ccnt,sync_mode,event_queue
 * Return Value        	 -0 - Succes, -1 - Failure
 * Note                 		-  None
 */


/* 2 DMA Channels Chained, Mem-2-Mem Copy, A-SYNC Mode, INCR Mode */
int kSt_edma_memtomemcpytest_chain(int acnt, int bcnt, int ccnt, int sync_mode,
				    int event_queue)
{
	int result = 0;
	unsigned int dma_ch1 = 0;
	unsigned int dma_ch2 = 0;
	unsigned int tcc1 = ST_EDMA_TCC_ANY;
	unsigned int tcc2 = ST_EDMA_TCC_ANY;
	int i;
	int count = 0;
	unsigned int Istestpassed1 = 0u;
	unsigned int Istestpassed2 = 0u;
	unsigned int numenabled = 0;
	unsigned int BRCnt = 0;
	int srcbidx = 0;
	int desbidx = 0;
	int srccidx = 0;
	int descidx = 0;
	st_edma_param_set param_set;
	s32 trial =0;

	for (trial = 0; trial <= MAX_TRIALS; trial++)
	{

		/* Initalize source and destination buffers */
		for (count = 0u; count < (acnt * bcnt * ccnt); count++) {
			dmabufsrc1[count] = 'A' + (count % 26);
			dmabufdest1[count] = 0;

			dmabufsrc2[count] = 'A' + (count % 26);
			dmabufdest2[count] = 0;
		}

		/* Set B count reload as B count. */
		BRCnt = bcnt;

		/* Setting up the SRC/DES Index */
		srcbidx = acnt;
		desbidx = acnt;

        	if (sync_mode == ASYNC) {
                	/* A Sync Transfer Mode */
                	srccidx = acnt;
                	descidx = acnt;
               		result = kSt_davinci_request_dma(ST_EDMA_CHANNEL_ANY, "A-SYNC_DMA0",
                        	                     kSt_callback1, NULL,&dma_ch1, &tcc1, event_queue);

        	} else if (sync_mode == ABSYNC) {
                	/* AB Sync Transfer Mode */
                	srccidx = acnt * bcnt;
                	descidx = acnt * bcnt;
                	result = kSt_davinci_request_dma(ST_EDMA_CHANNEL_ANY, "AB-SYNC_DMA0",
                        	                     kSt_callback1, NULL,&dma_ch1, &tcc1, event_queue);
        	} else {
                	TEST_PRINT_ERR(" Invalid Transfer mode \n");
        	}

		if (FAILURE == result) {
			TEST_PRINT_ERR("edma_test_chain::davinci_request_dma failed for dma_ch1, error:%d", result);
			return result;
		}

		kSt_davinci_set_dma_src_params(dma_ch1, (unsigned long)(dmaphyssrc1),
					   INCR, W8BIT);

		kSt_davinci_set_dma_dest_params(dma_ch1, (unsigned long)(dmaphysdest1),
					    INCR, W8BIT);

		kSt_davinci_set_dma_src_index(dma_ch1, srcbidx, srccidx);

		kSt_davinci_set_dma_dest_index(dma_ch1, desbidx, descidx);

	       if (sync_mode == ASYNC) {
        	        /* A Sync Transfer Mode */
                	kSt_davinci_set_dma_transfer_params(dma_ch1, acnt, bcnt, ccnt, BRCnt, ASYNC);
        	} else if (sync_mode == ABSYNC) {
                	/* AB Sync Transfer Mode */
                	kSt_davinci_set_dma_transfer_params(dma_ch1, acnt, bcnt, ccnt, BRCnt, ABSYNC);
        	} else {
                	TEST_PRINT_ERR (" Invalid Transfer mode \n");
        	}


      		if (sync_mode == ASYNC) {
			/* Request Another DMA Channel */
			result = kSt_davinci_request_dma(ST_EDMA_CHANNEL_ANY, "AB-SYNC_DMA0",
						     kSt_callback2, NULL,&dma_ch2, &tcc2, event_queue);
        	} else if (sync_mode == ABSYNC) {
                	/* Request Another DMA Channel */
                	result = kSt_davinci_request_dma(ST_EDMA_CHANNEL_ANY, "AB-SYNC_DMA0",
                       		                      kSt_callback2, NULL,&dma_ch2, &tcc2, event_queue);
        	} else {
                	TEST_PRINT_ERR(" Invalid Transfer mode \n");
        	}

		if (FAILURE == result) {
			TEST_PRINT_ERR("edma_test_chain::davinci_request_dma failed for dma_ch2, error:%d", result);

			kSt_davinci_free_dma(dma_ch1);
			return result;
		}

		kSt_davinci_set_dma_src_params(dma_ch2, (unsigned long)(dmaphyssrc2),
					   INCR, W8BIT);

		kSt_davinci_set_dma_dest_params(dma_ch2, (unsigned long)(dmaphysdest2),
					    INCR, W8BIT);

		kSt_davinci_set_dma_src_index(dma_ch2, srcbidx, srccidx);

		kSt_davinci_set_dma_dest_index(dma_ch2, desbidx, descidx);

       		if (sync_mode == ASYNC) {
                	/* A Sync Transfer Mode */
                	kSt_davinci_set_dma_transfer_params(dma_ch2, acnt, bcnt, ccnt, BRCnt, ASYNC);
        	} else if (sync_mode == ABSYNC) {
                	/* AB Sync Transfer Mode */
                	kSt_davinci_set_dma_transfer_params(dma_ch2, acnt, bcnt, ccnt, BRCnt, ABSYNC);
        	} else {
                	TEST_PRINT_ERR(" Invalid Transfer mode \n");
        	}

		/* Chain both the channels */
		
		/* Chain both the channels */
		kSt_davinci_get_dma_params(dma_ch1, &param_set);
		param_set.opt |= (1 << TCCHEN_SHIFT);
		param_set.opt |= EDMA_TCC(EDMA_CHAN_SLOT(dma_ch2));
		kSt_davinci_set_dma_params(dma_ch1, &param_set);
		
		/* Enable the Intermediate and Final Interrupts on Channel 1.
	 	* Also, Enable the Intermediate Chaining.
	 	*/
		kSt_davinci_get_dma_params(dma_ch1, &param_set);
		param_set.opt |= (1 << ITCCHEN_SHIFT);
		param_set.opt |= (1 << TCINTEN_SHIFT);
		param_set.opt |= (1 << ITCINTEN_SHIFT);
		kSt_davinci_set_dma_params(dma_ch1, &param_set);

		/* Enable the Intermediate and Final Interrupts on Channel 2 */
		kSt_davinci_get_dma_params(dma_ch2, &param_set);
		param_set.opt |= (1 << TCINTEN_SHIFT);
		param_set.opt |= (1 << ITCINTEN_SHIFT);
		kSt_davinci_set_dma_params(dma_ch2, &param_set);

        	if (sync_mode == ASYNC) {
                	numenabled = bcnt * ccnt;
        	} else if (sync_mode == ABSYNC) {
                	numenabled = ccnt;
        	} else {
                	TEST_PRINT_ERR (" Invalid Transfer mode \n");
        	}

		for (i = 0; i < numenabled; i++) {
			irqraised2 = 0;

			/* Now enable the transfer for Master channel as many times
		 	* as calculated above.
			 */
			result = kSt_davinci_start_dma(dma_ch1);
			if (result != 0) {
				TEST_PRINT_ERR("edma_test_chain: kSt_davinci_start_dma failed ");

				kSt_davinci_stop_dma(dma_ch1);
				kSt_davinci_free_dma(dma_ch1);
				kSt_davinci_free_dma(dma_ch2);
				return result;
			}

			/* Transfer on the master channel (ch1Id) will finish after some
			* time.
			* Now, because of the enabling of intermediate chaining on channel
			* 1, after the transfer gets over, a sync event will be sent
			* to channel 2, which will trigger the transfer on it.
			* Also, Final and Intermediate Transfer Complete
			* Interrupts are enabled on channel 2, so we should wait for the
			* completion ISR on channel 2 first, before proceeding
			* ahead.
			*/
			while (irqraised2 == 0u) ;

			/* Check the status of the completed transfer */
			if (irqraised2 < 0) {
				/* Some error occured, break from the FOR loop. */
				TEST_PRINT_ERR("edma3_test_with_chaining: "
					   "Event Miss Occured!!!");
				break;
			}

		}

		/* Match the Source and Destination Buffers. */
		for (i = 0; i < (acnt * bcnt * ccnt); i++) {
			if (dmabufsrc1[i] != dmabufdest1[i]) {
				TEST_PRINT_ERR("edma_test_chain(1): "
					"Data write-read matching failed at = %u",i);
				Istestpassed1 = 0u;
				result = -1;
				break;
			}
		}
		if (i == (acnt * bcnt * ccnt)) {
			Istestpassed1 = 1u;
		}

		for (i = 0; i < (acnt * bcnt * ccnt); i++) {
			if (dmabufsrc2[i] != dmabufdest2[i]) {
				TEST_PRINT_ERR("edma_test_chain(2): "
					"Data write-read matching failed at = %u",i);
				Istestpassed2 = 0u;
				result = -1;
				break;
			}
		}
		if (i == (acnt * bcnt * ccnt)) {
			Istestpassed2 = 1u;
		}

		kSt_davinci_stop_dma(dma_ch1);
		kSt_davinci_free_dma(dma_ch1);

		kSt_davinci_stop_dma(dma_ch2);
		kSt_davinci_free_dma(dma_ch2);
	}

	if ((Istestpassed1 == 1u) && (Istestpassed2 == 1u)) {
		DEBUG_PRINT("edma_test_chain: "
			"Transfer controller/event_queue: %d", event_queue);
		DEBUG_PRINT("edma_test_chain: "
			"Mode: %d  0 -> ASYNC, 1 -> ABSYNC", sync_mode);	
		TEST_PRINT_TRC("edma_test_chain: "
	   	 	"EDMA Data Transfer Successfull on TC %d",event_queue);
	} else {
		TEST_PRINT_ERR("edma_test_chain: EDMA Data Transfer Failed");
	}
	return result;
}

