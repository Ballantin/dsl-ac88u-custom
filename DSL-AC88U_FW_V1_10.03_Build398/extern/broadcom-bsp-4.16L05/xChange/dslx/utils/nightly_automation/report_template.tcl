set reportXml {
<RGE>
    <Section type="root" name="Root">
        <Parameters>
            <Param name="report-format-pdf">Y</Param>
            <Param name="report-format-html">N</Param>
            <Param name="report-format-xml">Y</Param>
            <Param name="report-file-name-root">MyReport</Param>
            <Param name="report-auto-gen-end-of-test">Y</Param>
        </Parameters>
        <Section type="report-title" name="Report Title">
            <Parameters>
                <Param name="title-company">Broadcom</Param>
                <Param name="title-test-title">NightBuild Automated Bulkcall Test</Param>
                <Param name="title-user-name">Dslx</Param>
                <Param name="logo-choose">S</Param>
                <Param name="print-place">1</Param>
            </Parameters>
        </Section>
        <Section type="events" name="Events">
            <Parameters>
                <Param name="show-real-time">Y</Param>
                <Param name="show-elapsed-time">Y</Param>
                <Param name="show-event-type">Y</Param>
                <Param name="show-comment">Y</Param>
                <Param name="show-phone">Y</Param>
                <Param name="show-cause">Y</Param>
            </Parameters>
        </Section>
        <Section type="partition-timing" name="Partition and Timing"/>
        <Section type="channel-selection" name="Channel Selection">
            <ChannelSelection selection-method="ALL" include-originate="YES" include-terminate="YES" averaged="NO"/>
            <Section type="statistics" name="Statistics"/>
        </Section>
        <Section type="scripts" name="Scripts"/>
    </Section>
</RGE>}
